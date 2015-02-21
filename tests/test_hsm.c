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

#include "mbb/test.h"
#include "mbb/hsm.h"
#include "mbb/debug.h"
#include "mbb/queue.h"

MHSM_DEFINE_STATE(test_it_a, NULL);
MHSM_DEFINE_STATE(test_it_a1, &test_it_a);
MHSM_DEFINE_STATE(test_it_a11, &test_it_a1);

mhsm_state_t *test_it_a_fun(mhsm_hsm_t *hsm, mhsm_event_t event)
{
	switch (event.id) {
		case MHSM_EVENT_INITIAL:
			return &test_it_a1;
	}

	return &test_it_a;
}

mhsm_state_t *test_it_a1_fun(mhsm_hsm_t *hsm, mhsm_event_t event)
{
	switch (event.id) {
		case MHSM_EVENT_INITIAL:
			return &test_it_a11;
	}

	return &test_it_a1;
}

mhsm_state_t *test_it_a11_fun(mhsm_hsm_t *hsm, mhsm_event_t event)
{
	switch (event.id) {
		case MHSM_EVENT_ENTRY:
			MDBG_PRINT_LN("a11 entry");
			break;
	}

	return &test_it_a11;
}

char *test_initial_transition()
{
	mhsm_hsm_t hsm;

	mhsm_initialise(&hsm, NULL, &test_it_a);
	mhsm_dispatch_event(&hsm, MHSM_EVENT_INITIAL);

	MUNT_ASSERT(mhsm_current_state(&hsm) == &test_it_a11);

	return 0;
}

typedef struct {
	mhsm_state_t *state;
	uint32_t event_id;
} test_event_t;

MQUE_DEFINE_STRUCT(test_event_t, 30) test_event_queue;
MQUE_DEFINE_STRUCT(test_event_t, 30) expected_event_queue;

#define TEST_ENQUEUE(STATE, EVENT_ID) do { \
	test_event_t test_event; \
	test_event.state = STATE; \
	test_event.event_id = EVENT_ID; \
	MQUE_ENQUEUE(&test_event_queue, test_event); \
} while (0)

#define EXPECTED_ENQUEUE(STATE, EVENT_ID) do { \
	test_event_t expected_event; \
	expected_event.state = STATE; \
	expected_event.event_id = EVENT_ID; \
	MQUE_ENQUEUE(&expected_event_queue, expected_event); \
} while (0)

enum {
	TEST_TE_EVENT_TRIGGER = MHSM_EVENT_CUSTOM
};
 
MHSM_DEFINE_STATE(test_te_top, NULL);
MHSM_DEFINE_STATE(test_te_a, &test_te_top);
MHSM_DEFINE_STATE(test_te_a1, &test_te_a);
MHSM_DEFINE_STATE(test_te_b, &test_te_top);
MHSM_DEFINE_STATE(test_te_b1, &test_te_b);

mhsm_state_t *test_te_top_fun(mhsm_hsm_t *hsm, mhsm_event_t event)
{
	TEST_ENQUEUE(&test_te_top, event.id);

	return &test_te_top;
}

mhsm_state_t *test_te_a_fun(mhsm_hsm_t *hsm, mhsm_event_t event)
{
	TEST_ENQUEUE(&test_te_a, event.id);

	switch (event.id) {
		case MHSM_EVENT_INITIAL:
			return &test_te_a1;
	}

	return &test_te_a;
}

mhsm_state_t *test_te_a1_fun(mhsm_hsm_t *hsm, mhsm_event_t event)
{
	TEST_ENQUEUE(&test_te_a1, event.id);

	switch (event.id) {
		case TEST_TE_EVENT_TRIGGER:
			return &test_te_b1;
	}

	return &test_te_a1;
}

mhsm_state_t *test_te_b_fun(mhsm_hsm_t *hsm, mhsm_event_t event)
{
	TEST_ENQUEUE(&test_te_b, event.id);

	switch (event.id) {
		case MHSM_EVENT_INITIAL:
			return &test_te_b1;
	}

	return &test_te_b;
}

mhsm_state_t *test_te_b1_fun(mhsm_hsm_t *hsm, mhsm_event_t event)
{
	TEST_ENQUEUE(&test_te_b1, event.id);

	switch (event.id) {
		case MHSM_EVENT_ENTRY:
			MDBG_PRINT_LN("entry b1");
			break;
	}

	return &test_te_b1;
}

char *test_transition_events()
{
	mhsm_hsm_t hsm;

	MQUE_INITIALISE(&expected_event_queue);
	MQUE_INITIALISE(&test_event_queue);

	EXPECTED_ENQUEUE(&test_te_top, MHSM_EVENT_ENTRY);
	EXPECTED_ENQUEUE(&test_te_a, MHSM_EVENT_ENTRY);
	EXPECTED_ENQUEUE(&test_te_a, MHSM_EVENT_INITIAL);
	EXPECTED_ENQUEUE(&test_te_a1, MHSM_EVENT_ENTRY);
	EXPECTED_ENQUEUE(&test_te_a1, MHSM_EVENT_INITIAL);
	EXPECTED_ENQUEUE(&test_te_a1, TEST_TE_EVENT_TRIGGER);
	EXPECTED_ENQUEUE(&test_te_a, TEST_TE_EVENT_TRIGGER);
	EXPECTED_ENQUEUE(&test_te_top, TEST_TE_EVENT_TRIGGER);
	EXPECTED_ENQUEUE(&test_te_a1, MHSM_EVENT_EXIT);
	EXPECTED_ENQUEUE(&test_te_a, MHSM_EVENT_EXIT);
	EXPECTED_ENQUEUE(&test_te_b, MHSM_EVENT_ENTRY);
	EXPECTED_ENQUEUE(&test_te_b1, MHSM_EVENT_ENTRY);
	EXPECTED_ENQUEUE(&test_te_b1, MHSM_EVENT_INITIAL);

	mhsm_initialise(&hsm, NULL, &test_te_a);
	mhsm_dispatch_event(&hsm, MHSM_EVENT_INITIAL);
	mhsm_dispatch_event(&hsm, TEST_TE_EVENT_TRIGGER);

	MUNT_ASSERT(MQUE_LENGTH(&expected_event_queue) == MQUE_LENGTH(&test_event_queue));

	while (MQUE_LENGTH(&expected_event_queue)) {
		test_event_t expected_event = MQUE_HEAD(&expected_event_queue);
		test_event_t test_event = MQUE_HEAD(&test_event_queue);

		MUNT_ASSERT(expected_event.state == test_event.state && expected_event.event_id == test_event.event_id);
		MQUE_DEQUEUE(&expected_event_queue);
		MQUE_DEQUEUE(&test_event_queue);
	}

	return 0;
}
