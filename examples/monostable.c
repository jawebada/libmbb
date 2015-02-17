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

#include "mbb/hsm.h"
#include "mbb/timer_ev.h"
#include "mbb/debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <ev.h>

#define MONO_NROF_SWITCHES	5
#define MONO_TIMEOUT		(2 * MTMR_ONE_SEC)
#define MONO_PERIOD		(100 * MTMR_ONE_MSEC)

enum {
	/* timeout events first */
	MONO_EVENT_TIMEOUT = MHSM_EVENT_CUSTOM,
	MONO_EVENT_TRIGGER
};

typedef struct {
	/* the state structure must start with an array of timers */
	mtmr_ev_t timer[MTMR_NROF_TIMERS(MONO_EVENT_TIMEOUT)];
	int id;
	int counter;
} mono_state_t;

MHSM_DEFINE_STATE(mono_off, NULL);
MHSM_DEFINE_STATE(mono_on, NULL);

mhsm_state_t *mono_off_fun(mhsm_hsm_t* hsm, mhsm_event_t event)
{
	mono_state_t *state = (mono_state_t*) mhsm_context(hsm);

	switch (event.id) {
		case MHSM_EVENT_ENTRY:
			MDBG_PRINT2("OFF %02d (%03d)\n", state->id, state->counter);
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
			state->counter++;
			/* fall-through */
		case MONO_EVENT_TRIGGER:
			MDBG_PRINT2("ON  %02d (%03d)\n", state->id, state->counter);
			mhsm_start_timer(hsm, MONO_EVENT_TIMEOUT, MONO_TIMEOUT);
			break;
		case MONO_EVENT_TIMEOUT:
			return &mono_off;
	}

	return &mono_on;
}

static void print_cb(EV_P_ ev_timer *w, int revents)
{
	mhsm_hsm_t *switches = (mhsm_hsm_t*) w->data;
	int i;

	printf("\r");
	for (i = 0; i < MONO_NROF_SWITCHES; i++) {
		mhsm_hsm_t *hsm = switches + i;
		mono_state_t *state = (mono_state_t*) mhsm_context(hsm);

		printf("%d: %s (%03d)\t", state->id, mhsm_is_in(hsm, &mono_on) ? "ON " : "OFF", state->counter);
	}
	fflush(stdout);
		
}

static void keypress_cb(EV_P_ ev_io *w, int revents)
{
	mhsm_hsm_t *switches = (mhsm_hsm_t*) w->data;
	char c;

	c = getchar();

	if (c >= '1' && c < '1' + MONO_NROF_SWITCHES) {
		mhsm_dispatch_event(switches + (c - '1'), MONO_EVENT_TRIGGER);
	}

	if (c == 'q') ev_break(EV_A_ EVBREAK_ALL);
}

#include "keyboard.inc"

int main(void)
{
	mhsm_hsm_t switches[MONO_NROF_SWITCHES];
	mono_state_t switch_states[MONO_NROF_SWITCHES];
	ev_io stdin_watcher;
	ev_timer print_timeout;
	struct ev_loop *loop = EV_DEFAULT;
	int i;

	for (i = 0; i < MONO_NROF_SWITCHES; i++) {
		mhsm_hsm_t *switch_hsm = switches + i;
		mono_state_t *switch_state = switch_states + i;

		mhsm_initialise(switch_hsm, switch_state, &mono_off);
		mtmr_ev_initalise_timers(switch_hsm, MTMR_NROF_TIMERS(MONO_EVENT_TIMEOUT), loop);

		switch_state->id = i + 1;
		switch_state->counter = 0;

		mhsm_dispatch_event(switch_hsm, MHSM_EVENT_INITIAL);
	}

	ev_io_init(&stdin_watcher, keypress_cb, 0, EV_READ);
	ev_io_start(loop, &stdin_watcher);
	stdin_watcher.data = switches;

	ev_timer_init(&print_timeout, print_cb, 0.1, 0.1);
	ev_timer_start(loop, &print_timeout);
	print_timeout.data = switches;

	nonblock(1);
	printf("press '1' to '%c' to enable lights, 'q' to quit\n", '1' + MONO_NROF_SWITCHES - 1);
	ev_run(loop, 0);
	printf("\nquitting\n");
	nonblock(0);

	return 0;
}
