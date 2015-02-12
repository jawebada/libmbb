#ifndef MBB_TIMER_H
#define MBB_TIMER_H

#include "types.h"
#include "hsm.h"

typedef struct {
	uint32_t period;
	uint32_t value;
	bool active;
} mtmr_t;

#define MTMR_NROF_TIMERS(LAST_TIMER_EVENT) (LAST_TIMER_EVENT - MHSM_EVENT_CUSTOM + 1)

int mtmr_initialise_timers(mhsm_hsm_t *hsm, uint32_t last_timer_event);
int mtmr_increment_timers(mhsm_hsm_t *hsm, uint32_t last_timer_event, uint32_t passed_msecs);

int mtmr_start_timer(mhsm_hsm_t *hsm, uint32_t event, uint32_t period_msecs);

#define MTMR_ONE_MSEC	(1)
#define MTMR_ONE_SEC	(1000 * MTMR_ONE_MSEC)
#define MTMR_ONE_MIN	(60 * MTMR_ONE_SEC)

#endif /* MBB_TIMER_H */
