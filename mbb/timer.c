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
