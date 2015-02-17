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
