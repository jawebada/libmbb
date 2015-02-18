libmbb - Hierarchical State Machines
====================================

*libmbb* features a lightweight module for hierarchical state machines (HSMs)
modeled after the 
[UML state machine](http://en.wikipedia.org/wiki/UML_state_machine).

HSM types, macros, and functions prototypes are defined in `mbb/hsm.h`. 

	#include "mbb/hsm.h"

Features
--------

* Composite states
* Run-to-completion processing
* Greedy transition selection
* DO event
* Timers with system-specific backends

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
[PEdestrian LIght CONtrolled (PELICAN) Crossing
Example](http://www.state-machine.com/resources/AN_PELICAN.pdf).

Events
------

	typedef struct {
		uint32_t id;
		int32_t arg;
	} mhsm_event_t;

`mhsm_event_t` represents an event which is identified by its `uint32_t id` and
features an optional `int32_t` argument `arg`.

There are five pre-defined event ids:

* `MHSM_EVENT_ENTRY`
* `MHSM_EVENT_INITIAL`
* `MHSM_EVENT_DO`
* `MHSM_EVENT_EXIT`
* `MHSM_EVENT_CUSTOM`

Typical examples for event arguments include return codes, error codes, and
indices. If an event is associated with an argument which cannot be represented
by an `int32_t` value you will have to reserve space in HSM's context structure
(see below).

States and Event Processing Functions
-------------------------------------

	typedef struct mhsm_state_s mhsm_state_t;

`mhsm_state_t` is an anonymous structure representing an HSM state. It is
basically a pointer to an event processing function along with a pointer to its
superstate.

	typedef mhsm_state_t *mhsm_event_processing_fun_t(mhsm_hsm_t *hsm, mhsm_event_t event);

An event processing function takes a pointer to the HSM and an event and
returns a pointer to the next state. The hsm pointer is needed to retrieve the
HSM's context and in case another event is to be dispatched. 

### State Transitions

If the pointer returned by the event processing function differs from the
current state of the HSM a transition is triggered. If more than one of the
active states trigger a transition the most inner state's transition is
performed.

A transition from `STATE_A` to `STATE_B` consists of the following steps:

* Find the least comon ancestor `LCA` of `STATE_A` and `STATE_B`
* Dispatch `MHSM_EVENT_EXIT` events starting at `STATE_A` up to but not
  including `LCA`
* Dispatch `MHSM_EVENT_ENTRY` events down to `STATE_B`
* Dispatch the `MHSM_EVENT_INITIAL` event to `STATE_B`

A composite state's event processing function can catch the
`MHSM_EVENT_INITIAL` event to trigger the initial transition to one of its
substates.

### Deferring Events

If a state cannot process a certain event its event processing function can
defer the event by returning `NULL`.  Deferred events are enqueued in an
HSM-specific queue. This queue's capacity is `MHSM_EVENT_QUEUE_LENGTH`, which
is 5 by default. Whether this is enough depends on the processing logic. If a
longer queue is needed `MHSM_EVENT_QUEUE_LENGTH` can be pre-defined before
including `mbb/hsm.h`.

Deferring an event also prevents any potential transitions triggered in super
states.

### Transitions Triggered by `MHSM_EVENT_ENTRY` and `MHSM_EVENT_EXIT` events

To ensure run-to-completion processing `MHSM_EVENT_ENTRY` and `MHSM_EVENT_EXIT`
events should usually not be allowed to trigger transitions. However,
*libmbb*'s HSM module allows such transitions which may be useful under certain
conditions.  

For example, a `MHSM_EVENT_EXIT` event may trigger an action which is supposed
to write some data to non-volatile memory. If this action fails it may be
necessary to interrupt the current transition and trigger a transition to a
fatal error state instead.

Use this feature with care as it can easily lead to infinite loops if abused.

### Defining States and Event Processing Functions

	MHSM_DEFINE_STATE(STATE, PARENT);

The event processing function for `STATE` is called `STATE_fun` by convention.
The macro `MHSM_DEFINE_STATE` is used to declare `STATE_fun`, define `STATE`,
and assign `STATE_fun` and `PARENT` to `STATE`'s internal pointers.

After defining the state hierarchy using `MHSM_DEFINE_STATE` the tool
`mhsm_scaffold` can be used to append event processing function stubs to the
source file:

	tools/mhsm_scaffold myhsm.c

The tool is rather primitive. It records all appearances of the
`MHSM_DEFINE_STATE` macro and appends an event processing function stub for
each of the states to the end of the file, regardless of any other existing
source code.

### Extended State Variables

Since event processing functions can be called by multiple HSM instances they
should not contain any static variables. If the application logic depends on
extended state variables these should be part of the HSM's context.

An HSM's context is typically represented by an HSM-specific context structure:

	typedef struct {
		...
	} hsm_context_t;

A pointer to such a state structure is given to the HSM during initialisation
(see below) and can be retrieved by event processing functions using the
function `mhsm_context`:

	void *mhsm_context(mhsm_hsm_t *hsm);

The `mhsm_scaffold` tool adds a variable pointing to the context structure to
the event processing function stubs if you add the `--context-type` option.

The generated event processing function stub including a context pointer looks
like this:

	mhsm_state_t *STATE_fun(mhsm_hsm_t *hsm, mhsm_event_t event)
	{
		hsm_context_t *ctx = (hsm_context_t*) mhsm_context(hsm);

		switch (event.id) {
			case MHSM_EVENT_ENTRY:
				break;
		}

		return &STATE;
	}

### Auxiliary Functions

The function `mhsm_is_ancestor` returns true if `ancestor` is an ancestor of
`target`:

	bool mhsm_is_ancestor(mhsm_state_t *ancestor, mhsm_state_t *target);

HSMs
----

	typedef struct mhsm_hsm_s mhsm_hsm_t;

`mhsm_hsm_t` is an anonymous structure representing an HSM. It stores the
pointer of the HSM's current state, a pointer the HSM's context, and the queue
of deferred events.

	void mhsm_initialise(mhsm_hsm_t *hsm, void *context, mhsm_state_t *initial_state);

The function `mhsm_initialise` must be called to initialse an HSM. In addition
to a pointer the HSM, it takes a pointer to HSM-specific context data and a
pointer to the HSM's initial state.

### Dispatching Events

After initialising the HSM events are dispatched using the functions
`mhsm_dispatch_event` and `mhsm_dispatch_event_arg`:

	void mhsm_dispatch_event(mhsm_hsm_t *hsm, uint32_t id);
	void mhsm_dispatch_event_arg(mhsm_hsm_t *hsm, uint32_t id, int32_t arg);

To trigger the initial transition you must dispatch the `MHSM_EVENT_INITIAL`
event:

	mhsm_dispatch_event(hsm, MHSM_EVENT_INITIAL);

### Recursive Calls of Dispatch Functions and Enqueued Events

The dispatch functions may be called from within an event processing function.
To assure run-to-completion processing the event is enqueued in the HSM's
internal queue in this case and dispatched after the processing function has
returned.

Each call of one of the dispatch functions will also dispatch all enqueued
events once after dispatching the given event.

### Auxiliary Functions

A pointer to the HSM's most inner active state can be retrieved using the
`mhsm_current_state` function:

	mhsm_state_t *mhsm_current_state(mhsm_hsm_t *hsm);

The function `mhsm_is_in` returns true if `state` is an active state (including
composite states):

	bool mhsm_is_in(mhsm_hsm_t *hsm, mhsm_state_t *state);

Timers
------

	int mhsm_start_timer(mhsm_hsm_t *hsm, uint32_t event_id, uint32_t period_msecs);
	void mhsm_set_timer_callback(int (*callback)(mhsm_hsm_t*, uint32_t, uint32_t));

Timer backends

* `mtmr_prd_t` for non-blocking real-time systems
* `mtmr_ev_t` based on [libev](http://software.schmorp.de/pkg/libev.html)

Non-Blocking versus Event-Driven Processing
-------------------------------------------

TBD
