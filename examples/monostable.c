/* * Copyright (C) 2015 Jan Weil
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "mbb/debug.h"
#include "mbb/hsm.h"
#include "mbb/timer.h"
#include <stdio.h>
#include <termios.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#define MONO_NROF_SWITCHES	5
#define MONO_TIMEOUT		(2 * MTMR_ONE_SEC)
#define MONO_PERIOD		(100 * MTMR_ONE_MSEC)

typedef struct {
	mtmr_t timers[1];
	int id;
	int counter;
} mono_state_t;

enum {
	MONO_EVENT_TIMEOUT = MHSM_EVENT_CUSTOM,
	MONO_EVENT_TRIGGER
};

MHSM_DEFINE_STATE(mono_top, NULL);
MHSM_DEFINE_STATE(mono_off, &mono_top);
MHSM_DEFINE_STATE(mono_on, &mono_top);

mhsm_state_t *mono_top_fun(mhsm_hsm_t *hsm, mhsm_event_t event)
{
	mono_state_t *state = (mono_state_t*) mhsm_context(hsm);

	switch (event.id) {
		case MHSM_EVENT_DO:
			printf(" %02d (%03d)\t", state->id, state->counter);
			break;
	}

	return &mono_top;
}

mhsm_state_t *mono_off_fun(mhsm_hsm_t* hsm, mhsm_event_t event)
{
	mono_state_t *state = (mono_state_t*) mhsm_context(hsm);

	switch (event.id) {
		case MHSM_EVENT_ENTRY:
			MDBG_PRINT1("%d off\n", state->id);
			break;
		case MHSM_EVENT_DO:
			printf("OFF");
			break;
		case MONO_EVENT_TRIGGER:
			return &mono_on;
	}

	return &mono_off;
}

mhsm_state_t *mono_on_fun(mhsm_hsm_t *hsm, mhsm_event_t event)
{
	mono_state_t *state = (mono_state_t*) mhsm_context(hsm);

	switch (event.id) {
		case MHSM_EVENT_ENTRY:
			MDBG_PRINT1("%d on\n", state->id);
			state->counter++;
			/* fall-through */
		case MHSM_EVENT_DO:
			printf("ON ");
			break;
		case MONO_EVENT_TRIGGER:
			mhsm_start_timer(hsm, MONO_EVENT_TIMEOUT, MONO_TIMEOUT);
			break;
		case MONO_EVENT_TIMEOUT:
			return &mono_off;
	}

	return &mono_on;
}

static void nonblock(int state)
{
	struct termios ttystate;

	tcgetattr(STDIN_FILENO, &ttystate);

	if (state) {
		ttystate.c_lflag &= ~ICANON;
		ttystate.c_cc[VMIN] = 1;
	} else {
		ttystate.c_lflag |= ICANON;
	}

	tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);

}

static int kbhit()
{
	struct timeval tv;
	fd_set fds;

	tv.tv_sec = 0;
	tv.tv_usec = 0;

	FD_ZERO(&fds);
	FD_SET(STDIN_FILENO, &fds); 

	select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
	return FD_ISSET(STDIN_FILENO, &fds);
}

int main(void)
{
	mhsm_hsm_t switches[MONO_NROF_SWITCHES];
	mono_state_t switch_states[MONO_NROF_SWITCHES];
	int i;

	for (i = 0; i < MONO_NROF_SWITCHES; i++) {
		mhsm_initialise(switches + i, switch_states + i, &mono_off);
		if (mtmr_initialise_timers(switches + i, MONO_EVENT_TIMEOUT) != 0) {
			MDBG_PRINT1("failed to initialise timers for hsm %d\n", i);
			exit(EXIT_FAILURE);
		}

		switch_states[i].id = i + 1;
		switch_states[i].counter = 0;

		mhsm_dispatch_event(switches + i, MHSM_EVENT_INITIAL);
	}

	nonblock(1);

	while (1) {
		usleep(MONO_PERIOD * 1000);

		if (kbhit()) {
			char c;

			c = fgetc(stdin);

			if (c >= '1' && c < ('1' + MONO_NROF_SWITCHES)) 
				mhsm_dispatch_event(switches + (c - '1'), MONO_EVENT_TRIGGER);

			if (c == 'q') break;
		}

		printf("\r");

		for (i = 0; i < MONO_NROF_SWITCHES; i++) {
			mtmr_increment_timers(switches + i, MONO_EVENT_TIMEOUT, MONO_PERIOD);
			mhsm_dispatch_event(switches + i, MHSM_EVENT_DO);
		}

		fflush(stdout);
	}

	printf("\nquitting\n");

	nonblock(0);

	return 0;
}