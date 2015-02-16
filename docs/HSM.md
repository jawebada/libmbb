libmbb - Hierarchical State Machines
====================================

*libmbb* features a lightweight module for hierarchical state machines (HSMs)
modeled after the 
[UML state machine](http://en.wikipedia.org/wiki/UML_state_machine).

HSM types, macros, and functions prototypes are defined in `mbb/hsm.h`. 

	#include "mbb/hsm.h"

Synopsis
--------

* Define custom events.
* Define the state hierarchy of your HSM.
* Optionally, use [`mhsm_scaffold`](../tools/mhsm_scaffold) to generate stubs
  of the event processing functions.
* Define the event processing functions.
* Create as many instances of your HSM as you need along with their contexts.
* Initialise your HSM instances and their contexts.
* Dispatch the `MHSM_EVENT_INITIAL` event to trigger the initial transition.
* Dispatch events as they occur.

Example
-------

	#include "mbb/hsm.h"
	#include <stdio.h>
	
	enum {
		EVENT1 = MHSM_EVENT_CUSTOM,
		EVENT2
	}
	
	MHSM_DEFINE_STATE(top, NULL);
	MHSM_DEFINE_STATE(a, &top);
	MHSM_DEFINE_STATE(b, &top);
	
	mhsm_state_t *top_fun(mhsm_hsm_t *hsm, mhsm_event_t event)
	{
		switch (event.id) {
			case EVENT1:
				printf("event1");
				break;
			case EVENT2:
				printf("event2");
				break;
		}
	
		return &top;
	}
	
	mhsm_state_t *a_fun(mhsm_hsm_t *hsm, mhsm_event_t event)
	{
		switch (event.id) {
			case EVENT1:
				return &b;
		}
	
		return &a;
	}
	
	mhsm_state_t *b_fun(mhsm_hsm_t *hsm, mhsm_event_t event)
	{
		switch (event.id) {
			case EVENT1:
				return &a;
		}
	
		return &b;
	}
	
	int main(void)
	{
		mhsm_hsm_t hsm;
	
		mhsm_initialise(&hsm, NULL, &a);
		mhsm_dispatch_event(&hsm, MHSM_EVENT_INITIAL);
		mhsm_dispatch_event(&hsm, EVENT1);
		mhsm_dispatch_event(&hsm, EVENT1);
		mhsm_dispatch_event(&hsm, EVENT2);
	
		return 0;
	}

* [monostable](../examples/monostable.c) is another simple example implementing
monostable multivibrators (as in staircase lightings).
* [pelican](../examples/pelican.c) is an implementation of 
[Miro Samek's](http://www.state-machine.com/)
[PEdestrian LIght CONtrolled (PELICAN) Crossing Example](http://www.state-machine.com/resources/AN_PELICAN.pdf).

States
------

	typedef struct mhsm_state_s mhsm_state_t;

`mhsm_state_t` is an anonymous structure representing an HSM state. It is
basically a pointer to an event processing function along with a pointer to its
superstate.

HSMs
----

	typedef struct mhsm_hsm_s mhsm_hsm_t;

`mhsm_hsm_t` is an anonymous structure representing an HSM. It stores the
pointer of the HSM's current state and a pointer the HSM's context.

Events
------

	typedef struct {
		uint32_t id;
		int32_t arg;
	} mhsm_event_t;

`mhsm_event_t` represents an Event which is identified by its `uint32_t` id and
features an optional `int32_t` argument. 

There are five pre-defined event ids:

* `MHSM_EVENT_ENTRY`
* `MHSM_EVENT_INITIAL`
* `MHSM_EVENT_DO`
* `MHSM_EVENT_EXIT`
* `MHSM_EVENT_CUSTOM`

Event Processing Functions
--------------------------

	typedef mhsm_state_t *mhsm_event_processing_fun_t(mhsm_hsm_t *hsm, mhsm_event_t event);

An event processing function takes a pointer to the HSM and an event and
returns a pointer to the next state. If this pointer differs from the current
state of the HSM a transition is triggered. The hsm pointer is needed to
retrieve the HSM's context and in case another event is to be dispatched.

Functions and Macros
--------------------

	MHSM_DEFINE_STATE(STATE, PARENT)

	void mhsm_initialise(mhsm_hsm_t *hsm, void *context, mhsm_state_t *initial_state);
	void mhsm_dispatch_event(mhsm_hsm_t *hsm, uint32_t id);
	void mhsm_dispatch_event_arg(mhsm_hsm_t *hsm, uint32_t id, int32_t arg);

	void *mhsm_context(mhsm_hsm_t *hsm);
	mhsm_state_t *mhsm_current_state(mhsm_hsm_t *hsm);
	bool mhsm_is_ancestor(mhsm_state_t *ancestor, mhsm_state_t *target);
	bool mhsm_is_in(mhsm_hsm_t *hsm, mhsm_state_t *state);

	void mhsm_set_timer_callback(int (*callback)(mhsm_hsm_t*, uint32_t, uint32_t));
	int mhsm_start_timer(mhsm_hsm_t *hsm, uint32_t event_id, uint32_t period_msecs);

Non-blocking versus event-driven processing
-------------------------------------------

TBD

Timers
------

TBD
