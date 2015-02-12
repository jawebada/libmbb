#ifndef MBB_HSM_H
#define MBB_HSM_H

/* Public API */
#include "types.h"
#include "queue.h"

/* HSM, state, and event types */
typedef struct mhsm_hsm_s mhsm_hsm_t;
typedef struct mhsm_state_s mhsm_state_t;
typedef struct {
	uint32_t id;
	int32_t arg;
} mhsm_event_t;
typedef mhsm_state_t *mhsm_event_processing_fun_t(mhsm_hsm_t *hsm, mhsm_event_t event);

#ifndef NDEBUG
# define MHSM_DEFINE_STATE(STATE, PARENT) \
  const char STATE##_name[] = #STATE; \
  mhsm_state_t *STATE##_fun(mhsm_hsm_t *hsm, mhsm_event_t event); \
  mhsm_state_t STATE = { STATE##_fun, PARENT, NULL, STATE##_name }
#else /* NDEBUG */
# define MHSM_DEFINE_STATE(STATE, PARENT) \
  mhsm_state_t *STATE##_fun(mhsm_hsm_t *hsm, mhsm_event_t event); \
  mhsm_state_t STATE = { STATE##_fun, PARENT, NULL, }
#endif


/* Common events */
enum {
	MHSM_EVENT_ENTRY,
	MHSM_EVENT_INITIAL,
	MHSM_EVENT_DO,
	MHSM_EVENT_EXIT,
	MHSM_EVENT_CUSTOM
};

void mhsm_initialise(mhsm_hsm_t *hsm, void *context, mhsm_state_t *initial_state);
void mhsm_dispatch_event(mhsm_hsm_t *hsm, uint32_t id);
void mhsm_dispatch_event_arg(mhsm_hsm_t *hsm, uint32_t id, int32_t arg);
void *mhsm_context(mhsm_hsm_t *hsm);
mhsm_state_t *mhsm_current_state(mhsm_hsm_t *hsm);
bool mhsm_is_ancestor(mhsm_state_t *ancestor, mhsm_state_t *target);
bool mhsm_is_in(mhsm_hsm_t *hsm, mhsm_state_t *state);
int mhsm_start_timer(mhsm_hsm_t *hsm, uint32_t event_id, uint32_t period_msecs);

#ifndef MHSM_EVENT_QUEUE_LENGTH
# define MHSM_EVENT_QUEUE_LENGTH 5
#endif

/* Private API */
#include "queue.h"

/* HSM struct */
struct mhsm_hsm_s {
	void *context;
	mhsm_state_t *current_state;
	MQUE_DEFINE_STRUCT(mhsm_event_t, MHSM_EVENT_QUEUE_LENGTH) deferred_events;
	bool in_transition;
	int (*start_timer_callback)(mhsm_hsm_t *hsm, uint32_t event_id, uint32_t period_msecs);
};

/* state struct */
struct mhsm_state_s {
	/* The state's event processing function */
	mhsm_event_processing_fun_t *event_processing_function;
	/* s.parent != NULL => s is a substate */
	mhsm_state_t *parent;
	/* s.current_substate != NULL => dispatch entry event to current_substate */
	mhsm_state_t *current_substate;
#ifndef NDEBUG
	const char *name;
#endif
};

#endif /* MBB_HSM_H */
