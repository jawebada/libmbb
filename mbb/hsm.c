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

#include "hsm.h"
#include "types.h"
#include "queue.h"
#include "debug.h"

static mhsm_state_t *_find_least_common_ancestor(mhsm_state_t *a, mhsm_state_t *b)
{
	if (a == NULL || b == NULL)
		return NULL;

	while (a->parent != NULL) {
		if (mhsm_is_ancestor(a->parent, b)) 
			return a->parent;
		
		a = a->parent;
	}

	return NULL;
}

static mhsm_state_t *_enter_state(mhsm_hsm_t *hsm, mhsm_state_t *from, mhsm_state_t *to);

static mhsm_state_t *_local_dispatch(mhsm_hsm_t *hsm, mhsm_state_t *state, mhsm_event_t event)
{
	MDBG_ASSERT(state != NULL);
	if (state == NULL)
		return NULL;

#ifndef NDEBUG
	switch (event.id) {
		case MHSM_EVENT_INITIAL:
		case MHSM_EVENT_ENTRY:
		case MHSM_EVENT_DO:
		case MHSM_EVENT_EXIT:
			break;
		default:
			MDBG_PRINT3("dispatching event %s (%d) to state %s\n", 
					event.id == MHSM_EVENT_INITIAL ? "INITIAL" :
					event.id == MHSM_EVENT_ENTRY ? "ENTRY" :
					event.id == MHSM_EVENT_DO ? "DO" :
					event.id == MHSM_EVENT_EXIT ? "EXIT" :
					"CUSTOM",
					event.id, state->name);
	}
#endif

	return state->event_processing_function(hsm, event);
}

static mhsm_state_t *_transition(mhsm_hsm_t *hsm, mhsm_state_t *from, mhsm_state_t *to)
{
	mhsm_state_t *least_common_ancestor, *result;

	MDBG_ASSERT(to != NULL);
	if (to == NULL) 
		/* Run, Forrest, run! */
		return NULL;

	MDBG_PRINT2("transition from %s to %s\n", from->name, to->name);

	/* dispatch exit events */
	if (mhsm_is_ancestor(from, to)) {
		least_common_ancestor = from;
	} else {
		if (mhsm_is_ancestor(to, from))
			least_common_ancestor = to;
		else
			least_common_ancestor = _find_least_common_ancestor(from, to);

		while (from != least_common_ancestor) {
			mhsm_event_t event;

			event.id = MHSM_EVENT_EXIT;
			result = _local_dispatch(hsm, from, event);
			if (result != from) {
				MDBG_PRINT_S(result->name);
				return _transition(hsm, from, result);
			}

			from = from->parent;
		}
	}

	if (least_common_ancestor == to)
		return least_common_ancestor;

	return _enter_state(hsm, least_common_ancestor, to);
}

static mhsm_state_t *_enter_state(mhsm_hsm_t *hsm, mhsm_state_t *from, mhsm_state_t *to)
{
	mhsm_state_t *current;

	MDBG_ASSERT(mhsm_is_ancestor(from, to));
	if (!mhsm_is_ancestor(from, to))
		return from;

	MDBG_ASSERT(to != NULL);
	if (to == NULL)
		return from;

	if (to == from)
		return to;

	/* link path to target state */
	for (current = to; current->parent != from; current = current->parent) {
		current->parent->current_substate = current;
	}

	/* dispatch parent entry events */
	while (current != to) {
		mhsm_event_t event;
		mhsm_state_t *result;

		event.id = MHSM_EVENT_ENTRY;
		result = _local_dispatch(hsm, current, event);
		if (result != current) {
			MDBG_PRINT2("dispatching the entry event to %s triggered a new transition to %s\n", current->name, result->name);
			return _transition(hsm, current, result);
		}

		current = current->current_substate;
		current->parent->current_substate = NULL;
	} 

	/* initial transition */
	while (1) {
		mhsm_event_t event;
		mhsm_state_t *target; 

		event.id = MHSM_EVENT_ENTRY;
		target = _local_dispatch(hsm, to, event);
		if (target != to) {
			MDBG_PRINT2("transition interrupted by %s, new target: %s\n", to->name, target->name);
			return _transition(hsm, to, target);
		}

		event.id = MHSM_EVENT_INITIAL;
		target = _local_dispatch(hsm, to, event);
		if (target == to) 
			break;
		else
			MDBG_PRINT2("intial transition to %s in composite state %s\n", target->name, to->name);

		to = target;
	}

	return to;
}

static int _defer_event_arg(mhsm_hsm_t *hsm, uint32_t id, int32_t arg)
{
	mhsm_event_t event;

	if (MQUE_IS_FULL(&hsm->deferred_events)) {
		MDBG_PRINT_LN("event queue too short");
		return -1;
	}

	event.id = id;
	event.arg = arg;

	MQUE_ENQUEUE(&hsm->deferred_events, event);

	MDBG_PRINT3("defered event (%d, %d) in %s\n", (int) event.id, (int) event.arg, hsm->current_state->name);

	return 0;
}

static mhsm_state_t *_dispatch_event(mhsm_hsm_t *hsm, mhsm_state_t *state, mhsm_event_t event)
{
	mhsm_state_t *target = state;
	mhsm_state_t *current;
	mhsm_state_t *result;

	/* catch special INITIAL event */
	if (event.id == MHSM_EVENT_INITIAL) {
		return _enter_state(hsm, NULL, state);
	}

	/* dispatch event to all active states */
	for (current = state; current != NULL; current = current->parent) {
		result = _local_dispatch(hsm, current, event);

		/* greedy transition selection */
		if (result != current && target == state) 
			target = result;
	}

	/* check whether the event was deferred */
	if (target == NULL) {
		MDBG_PRINT2("event %d was defered by %s\n", event.id, state->name);

		/* (re-)enqueue event */
		if (_defer_event_arg(hsm, event.id, event.arg) != 0)
			MDBG_PRINT_LN("(re-)enqueing defered event failed");

		return state;
	}

	if (target != state) 
		return _transition(hsm, state, target);

	return state;
}

void mhsm_initialise(mhsm_hsm_t *hsm, void *context, mhsm_state_t *initial_state)
{
	MQUE_INITIALISE(&hsm->deferred_events);
	hsm->context = context;
	hsm->current_state = initial_state;
	hsm->in_transition = 0;
	hsm->start_timer_callback = NULL;
}

void mhsm_dispatch_event(mhsm_hsm_t *hsm, uint32_t id)
{
	mhsm_dispatch_event_arg(hsm, id, 0);
}

void mhsm_dispatch_event_arg(mhsm_hsm_t *hsm, uint32_t id, int32_t arg)
{	
	mhsm_event_t event;
	int i;
	int nevents;

	MDBG_ASSERT(hsm->current_state != NULL);

	if (hsm->in_transition) {
		_defer_event_arg(hsm, id, arg);
		return;
	}

	hsm->in_transition = 1;

	event.id = id;
	event.arg = arg;

	hsm->current_state = _dispatch_event(hsm, hsm->current_state, event);
	MDBG_ASSERT(hsm->current_state != NULL);

	nevents = MQUE_LENGTH(&hsm->deferred_events);
	for (i = 0; i < nevents; i++) {
		MDBG_ASSERT(!MQUE_IS_EMPTY(&hsm->deferred_events));
		if (MQUE_IS_EMPTY(&hsm->deferred_events)) break;

		hsm->current_state = _dispatch_event(hsm, hsm->current_state, MQUE_HEAD(&hsm->deferred_events));
		MDBG_ASSERT(hsm->current_state != NULL);

		MQUE_DEQUEUE(&hsm->deferred_events);
	}

	hsm->in_transition = 0;
}

void *mhsm_context(mhsm_hsm_t *hsm)
{
	return hsm->context;
}

mhsm_state_t *mhsm_current_state(mhsm_hsm_t *hsm)
{
	return hsm->current_state;
}

bool mhsm_is_ancestor(mhsm_state_t *ancestor, mhsm_state_t *target)
{
	MDBG_ASSERT(target != NULL);
	if (target == NULL)
		return 0;

	if (ancestor == NULL)
		return 1;

	while (target->parent != NULL) {
		if (target->parent == ancestor) 
			return 1;

		target = target->parent;
	}

	return 0;
}

bool mhsm_is_in(mhsm_hsm_t *hsm, mhsm_state_t *state)
{
	return mhsm_current_state(hsm) == state || mhsm_is_ancestor(state, mhsm_current_state(hsm));
}

int mhsm_start_timer(mhsm_hsm_t *hsm, uint32_t event_id, uint32_t period_msecs)
{
	if (hsm->start_timer_callback == NULL) {
		MDBG_PRINT_LN("start_timer_callback uninitialised");
		return -1;
	}

	return hsm->start_timer_callback(hsm, event_id, period_msecs);
}
