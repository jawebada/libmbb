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

/*
 * PElican LIght CONtrolled Crossing Example
 * http://www.state-machine.com/resources/AN_PELICAN.pdf
 */

#include "mbb/hsm.h"
#include "mbb/timer.h"
#include "mbb/debug.h"
#include <stdio.h>
#include <stdlib.h>

#define PELICAN_PERIOD				(MTMR_ONE_SEC / 20)
#define PELICAN_TIMEOUT_CARS_GREEN_MIN		(8 * MTMR_ONE_SEC)
#define PELICAN_TIMEOUT_CARS_YELLOW		(3 * MTMR_ONE_SEC)
#define PELICAN_TIMEOUT_PEDS_WALK		(3 * MTMR_ONE_SEC)
#define PELICAN_TIMEOUT_PEDS_FLASH		(MTMR_ONE_SEC / 5)
#define PELICAN_PEDS_NROF_FLASHES		10
#define PELICAN_TIMEOUT_OFF_FLASH		(MTMR_ONE_SEC / 2)

enum {
	PELICAN_CARS_BLANK,
	PELICAN_CARS_RED,
	PELICAN_CARS_YELLOW,
	PELICAN_CARS_GREEN
};

enum {
	PELICAN_PEDS_BLANK,
	PELICAN_PEDS_DONT_WALK,
	PELICAN_PEDS_WALK
};

enum {
	/* timeout events must be defined first */
	PELICAN_EVENT_TIMEOUT_CARS_GREEN_MIN = MHSM_EVENT_CUSTOM,
	PELICAN_EVENT_TIMEOUT_CARS_YELLOW,
	PELICAN_EVENT_TIMEOUT_PEDS_WALK,
	PELICAN_EVENT_TIMEOUT_PEDS_FLASH,
	PELICAN_EVENT_TIMEOUT_OFF_FLASH,
	PELICAN_EVENT_PEDS_WAITING,
	PELICAN_EVENT_OFF,
	PELICAN_EVENT_ON
};

MHSM_DEFINE_STATE(operational, NULL);
MHSM_DEFINE_STATE(cars_enabled, &operational);
MHSM_DEFINE_STATE(cars_green, &cars_enabled);
MHSM_DEFINE_STATE(cars_green_no_ped, &cars_green);
MHSM_DEFINE_STATE(cars_green_int, &cars_green);
MHSM_DEFINE_STATE(cars_green_ped_wait, &cars_green);
MHSM_DEFINE_STATE(cars_yellow, &cars_enabled);
MHSM_DEFINE_STATE(peds_enabled, &operational);
MHSM_DEFINE_STATE(peds_walk, &peds_enabled);
MHSM_DEFINE_STATE(peds_flash, &peds_enabled);
MHSM_DEFINE_STATE(offline, NULL);

typedef struct {
	/* 
	 * mtmr functions assume that mhsm_context(hsm) points to an array of
	 * mtmr_t. So this array must be the first component of the state
	 * structure. 
	 * If the timeout events are defined first (see above)
	 * MTMR_NROF_TIMERS(last_timer_event) gives the number of timers.
	 */
	mtmr_t timers[MTMR_NROF_TIMERS(PELICAN_EVENT_TIMEOUT_OFF_FLASH)];
	int cars_light_state;
	int peds_light_state;
	int peds_flash_counter;
} pelican_state_t;

static void print_state(pelican_state_t *state)
{
	int i;

	printf("\r");
	for (i = 0; i < 80; i++) printf(" ");

	printf("\r");
	printf("cars: ");
	switch (state->cars_light_state) {
		case PELICAN_CARS_RED:
			printf("red");
			break;
		case PELICAN_CARS_YELLOW:
			printf("yellow");
			break;
		case PELICAN_CARS_GREEN:
			printf("green");
			break;
		default:
			printf("    ");
			break;
	}
	printf("\tpedestrians: ");
	switch (state->peds_light_state) {
		case PELICAN_PEDS_WALK:
			printf("walk");
			break;
		case PELICAN_PEDS_DONT_WALK:
			printf("don't walk");
			break;
		default:
			printf("    ");
			break;
	}
	fflush(stdout);
}

static void set_cars_light(pelican_state_t *state, int light_state)
{
	state->cars_light_state = light_state;
	print_state(state);
}

static void set_peds_light(pelican_state_t *state, int light_state)
{
	state->peds_light_state = light_state;
	print_state(state);
}

mhsm_state_t *operational_fun(mhsm_hsm_t *hsm, mhsm_event_t event)
{
	pelican_state_t *state = (pelican_state_t*) mhsm_context(hsm);

	switch (event.id) {
		case MHSM_EVENT_ENTRY:
			set_cars_light(state, PELICAN_CARS_RED);
			set_peds_light(state, PELICAN_PEDS_DONT_WALK);
			state->peds_flash_counter = 0;
			break;
		case MHSM_EVENT_INITIAL:
			return &cars_enabled;
		case PELICAN_EVENT_OFF:
			return &offline;
	}

	return &operational;
}

mhsm_state_t *cars_enabled_fun(mhsm_hsm_t *hsm, mhsm_event_t event)
{
	pelican_state_t *state = (pelican_state_t*) mhsm_context(hsm);

	switch (event.id) {
		case MHSM_EVENT_INITIAL:
			return &cars_green;
		case MHSM_EVENT_EXIT:
			set_cars_light(state, PELICAN_CARS_RED);
			break;
	}

	return &cars_enabled;
}

mhsm_state_t *cars_green_fun(mhsm_hsm_t *hsm, mhsm_event_t event)
{
	pelican_state_t *state = (pelican_state_t*) mhsm_context(hsm);

	switch (event.id) {
		case MHSM_EVENT_ENTRY:
			set_cars_light(state, PELICAN_CARS_GREEN);
			break;
		case MHSM_EVENT_INITIAL:
			return &cars_green_no_ped;
	}

	return &cars_green;
}

mhsm_state_t *cars_green_no_ped_fun(mhsm_hsm_t *hsm, mhsm_event_t event)
{
	switch (event.id) {
		case MHSM_EVENT_ENTRY:
			mhsm_start_timer(hsm, PELICAN_EVENT_TIMEOUT_CARS_GREEN_MIN, PELICAN_TIMEOUT_CARS_GREEN_MIN);
			break;
		case PELICAN_EVENT_TIMEOUT_CARS_GREEN_MIN:
			return &cars_green_int;
		case PELICAN_EVENT_PEDS_WAITING:
			return &cars_green_ped_wait;
	}

	return &cars_green_no_ped;
}

mhsm_state_t *cars_green_int_fun(mhsm_hsm_t *hsm, mhsm_event_t event)
{
	switch (event.id) {
		case PELICAN_EVENT_PEDS_WAITING:
			return &cars_yellow;
	}

	return &cars_green_int;
}

mhsm_state_t *cars_green_ped_wait_fun(mhsm_hsm_t *hsm, mhsm_event_t event)
{
	switch (event.id) {
		case PELICAN_EVENT_TIMEOUT_CARS_GREEN_MIN:
			return &cars_yellow;
	}

	return &cars_green_ped_wait;
}

mhsm_state_t *cars_yellow_fun(mhsm_hsm_t *hsm, mhsm_event_t event)
{
	pelican_state_t *state = (pelican_state_t*) mhsm_context(hsm);

	switch (event.id) {
		case MHSM_EVENT_ENTRY:
			set_cars_light(state, PELICAN_CARS_YELLOW);
			mhsm_start_timer(hsm, PELICAN_EVENT_TIMEOUT_CARS_YELLOW, PELICAN_TIMEOUT_CARS_YELLOW);
			break;
		case PELICAN_EVENT_TIMEOUT_CARS_YELLOW:
			return &peds_enabled;
	}

	return &cars_yellow;
}

mhsm_state_t *peds_enabled_fun(mhsm_hsm_t *hsm, mhsm_event_t event)
{
	pelican_state_t *state = (pelican_state_t*) mhsm_context(hsm);

	switch (event.id) {
		case MHSM_EVENT_INITIAL:
			return &peds_walk;
		case MHSM_EVENT_EXIT:
			set_peds_light(state, PELICAN_PEDS_DONT_WALK);
			break;
	}

	return &peds_enabled;
}

mhsm_state_t *peds_walk_fun(mhsm_hsm_t *hsm, mhsm_event_t event)
{
	pelican_state_t *state = (pelican_state_t*) mhsm_context(hsm);

	switch (event.id) {
		case MHSM_EVENT_ENTRY:
			mhsm_start_timer(hsm, PELICAN_EVENT_TIMEOUT_PEDS_WALK, PELICAN_TIMEOUT_PEDS_WALK);
			set_peds_light(state, PELICAN_PEDS_WALK);
			break;
		case PELICAN_EVENT_TIMEOUT_PEDS_WALK:
			return &peds_flash;
	}

	return &peds_walk;
}

mhsm_state_t *peds_flash_fun(mhsm_hsm_t *hsm, mhsm_event_t event)
{
	pelican_state_t *state = (pelican_state_t*) mhsm_context(hsm);

	switch (event.id) {
		case MHSM_EVENT_ENTRY:
			state->peds_flash_counter = PELICAN_PEDS_NROF_FLASHES;
			set_peds_light(state, PELICAN_PEDS_BLANK);
			mhsm_start_timer(hsm, PELICAN_EVENT_TIMEOUT_PEDS_FLASH, PELICAN_TIMEOUT_PEDS_FLASH);
			break;
		case PELICAN_EVENT_TIMEOUT_PEDS_FLASH:
			state->peds_flash_counter--;
			if (state->peds_flash_counter == 0)
				return &cars_enabled;
			set_peds_light(state, (state->peds_flash_counter % 2) ? PELICAN_PEDS_WALK : PELICAN_PEDS_BLANK);
			mhsm_start_timer(hsm, PELICAN_EVENT_TIMEOUT_PEDS_FLASH, PELICAN_TIMEOUT_PEDS_FLASH);
			break;
	}

	return &peds_flash;
}

mhsm_state_t *offline_fun(mhsm_hsm_t *hsm, mhsm_event_t event)
{
	pelican_state_t *state = (pelican_state_t*) mhsm_context(hsm);

	switch (event.id) {
		case MHSM_EVENT_ENTRY:
			set_cars_light(state, PELICAN_CARS_RED);
			set_peds_light(state, PELICAN_PEDS_DONT_WALK);
			mhsm_start_timer(hsm, PELICAN_EVENT_TIMEOUT_OFF_FLASH, PELICAN_TIMEOUT_OFF_FLASH);
			break;
		case PELICAN_EVENT_TIMEOUT_OFF_FLASH:
			set_cars_light(state, state->cars_light_state == PELICAN_CARS_RED ? PELICAN_CARS_BLANK : PELICAN_CARS_RED);
			set_peds_light(state, state->peds_light_state == PELICAN_PEDS_DONT_WALK ? PELICAN_PEDS_BLANK : PELICAN_PEDS_DONT_WALK);
			mhsm_start_timer(hsm, PELICAN_EVENT_TIMEOUT_OFF_FLASH, PELICAN_TIMEOUT_OFF_FLASH);
			break;
		case PELICAN_EVENT_ON:
			return &operational;
	}

	return &offline;
}

#include "periodic.inc"
#include "keyboard.inc"

static int process(mhsm_hsm_t *pelican, void *state)
{
	if (kbhit()) {
		char c = fgetc(stdin);

		switch (c) {
			case ' ':
				mhsm_dispatch_event(pelican, PELICAN_EVENT_PEDS_WAITING);
				break;
			case 'o':
				mhsm_dispatch_event(pelican, mhsm_is_in(pelican, &operational) ? PELICAN_EVENT_OFF : PELICAN_EVENT_ON);
				break;
			case 'q':
				break;
			default:
				MDBG_PRINT1("unhandled key press: '%c'\n", c);
				break;
		}

		if (c == 'q') return -1;
	}

	mtmr_increment_timers(pelican, PELICAN_EVENT_TIMEOUT_OFF_FLASH, PELICAN_PERIOD);

	return 0;
}

int main(void)
{
	mhsm_hsm_t pelican;
	pelican_state_t pelican_state;

	printf("press [space] to trigger PEDS_WAITING event\n");
	printf("press 'o' to trigger ON/OFF events\n");
	printf("press 'q' to quit\n");

	mhsm_initialise(&pelican, &pelican_state, &operational);
	if (mtmr_initialise_timers(&pelican, PELICAN_EVENT_TIMEOUT_OFF_FLASH) != 0) {
		MDBG_PRINT_LN("failed to initialise timers");
		exit(EXIT_FAILURE);
	}
	mhsm_dispatch_event(&pelican, MHSM_EVENT_INITIAL);

	nonblock(1);
	periodic(PELICAN_PERIOD, process, &pelican, &pelican_state);
	nonblock(0);

	printf("\nquitting\n");

	return 0;
}
