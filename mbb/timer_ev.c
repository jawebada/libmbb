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

#include "timer_ev.h"
#include "types.h"
#include <ev.h>

static void timeout_cb(EV_P_ ev_timer *w, int revents)
{
	mtmr_ev_t *timer = (mtmr_ev_t*) w->data;

	ev_timer_stop(EV_A_ w);
	mhsm_dispatch_event(timer->hsm, timer->event_id);
}

int mtmr_ev_start_timer(mhsm_hsm_t *hsm, uint32_t event_id, uint32_t period_msecs)
{
	mtmr_ev_t *timers = (mtmr_ev_t*) mhsm_context(hsm);
	uint32_t idx = event_id - MHSM_EVENT_CUSTOM;
	mtmr_ev_t *timer = timers + idx;
	ev_timer *ev_timer = &timer->timer;
	ev_tstamp duration = (ev_tstamp) period_msecs / 1000.0;

	if (ev_is_active(ev_timer))
		ev_timer_stop(timer->loop, ev_timer);
	ev_timer_init(ev_timer, timeout_cb, duration, 0);
	ev_timer_start(timer->loop, ev_timer);

	return 0;
}

int mtmr_ev_initalise_timers(mhsm_hsm_t *hsm, uint32_t last_timer_event, struct ev_loop *loop)
{
	mtmr_ev_t *timers = (mtmr_ev_t*) mhsm_context(hsm);
	int i;

	for (i = 0; i < MTMR_NROF_TIMERS(last_timer_event); i++) {
		mtmr_ev_t *timer = timers + i;
		ev_timer *ev_timer = &timer->timer;

		timer->loop = loop;
		timer->hsm = hsm;
		timer->event_id = MHSM_EVENT_CUSTOM + i;
		ev_timer->data = timer;
		ev_timer_init(ev_timer, timeout_cb, 0, 0);
	}

	mhsm_set_timer_callback(hsm, mtmr_ev_start_timer);

	return 0;
}
