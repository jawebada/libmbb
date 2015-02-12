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

#include "timer.h"
#include "types.h"
#include "hsm.h"
#include "debug.h"

int mtmr_initialise_timers(mhsm_hsm_t *hsm, uint32_t last_timer_event)
{
	mtmr_t *timers;
	uint32_t i;

	if (hsm == NULL) return -1;

	timers = (mtmr_t*) mhsm_context(hsm);

	for (i = 0; i < MTMR_NROF_TIMERS(last_timer_event); i++) {
		mtmr_t *timer = timers + i;

		timer->active = 0;
		timer->period = 0;
		timer->value = 0;
	}

	hsm->start_timer_callback = mtmr_start_timer;

	return 0;
}

int mtmr_increment_timers(mhsm_hsm_t *hsm, uint32_t last_timer_event, uint32_t passed_msecs)
{
	mtmr_t *timers;
	uint32_t i;

	if (hsm == NULL) return -1;

	timers = (mtmr_t*) mhsm_context(hsm);

	for (i = 0; i < MTMR_NROF_TIMERS(last_timer_event); i++) {
		mtmr_t *timer = timers + i;

		if (timer->active) {
			timer->value += passed_msecs;
			if (timer->value >= timer->period) {
				mhsm_dispatch_event(hsm, MHSM_EVENT_CUSTOM + i);
				timer->active = 0;
			}
		}
	}

	return 0;
}

int mtmr_start_timer(mhsm_hsm_t *hsm, uint32_t event_id, uint32_t period_msecs)
{
	mtmr_t *timers = (mtmr_t*) mhsm_context(hsm);
	uint32_t idx = event_id - MHSM_EVENT_CUSTOM;

	timers[idx].period = period_msecs;
	timers[idx].value = 0;
	timers[idx].active = 1;

	return 0;
}
