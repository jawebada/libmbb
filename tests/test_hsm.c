#include "mbb/test.h"
#include "mbb/hsm.h"
#include "mbb/debug.h"

MHSM_DEFINE_STATE(state_a, NULL);
MHSM_DEFINE_STATE(state_a1, &state_a);
MHSM_DEFINE_STATE(state_a11, &state_a1);

mhsm_state_t *state_a_fun(mhsm_hsm_t *hsm, mhsm_event_t event)
{
	switch (event.id) {
		case MHSM_EVENT_INITIAL:
			return &state_a1;
	}

	return &state_a;
}

mhsm_state_t *state_a1_fun(mhsm_hsm_t *hsm, mhsm_event_t event)
{
	switch (event.id) {
		case MHSM_EVENT_INITIAL:
			return &state_a11;
	}

	return &state_a1;
}

mhsm_state_t *state_a11_fun(mhsm_hsm_t *hsm, mhsm_event_t event)
{
	switch (event.id) {
		case MHSM_EVENT_ENTRY:
			MDBG_PRINT_LN("a11 entry");
			break;
	}

	return &state_a11;
}

char *test_initial_transition()
{
	mhsm_hsm_t hsm;

	mhsm_initialise(&hsm, NULL, &state_a);
	mhsm_dispatch_event(&hsm, MHSM_EVENT_INITIAL);

	MUNT_ASSERT(mhsm_current_state(&hsm) == &state_a11);

	return 0;
}
