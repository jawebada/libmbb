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
#include "mbb/queue.h"
#include "mbb/debug.h"

char *test_enqueue_dequeue()
{
	int i;

	MQUE_DEFINE_STRUCT(int, 5) queue;

	MQUE_INITIALISE(&queue);

	MUNT_ASSERT(MQUE_IS_EMPTY(&queue));

	for (i = 1; i <= 5; i++) {
		MUNT_ASSERT(!MQUE_IS_FULL(&queue));
		MQUE_ENQUEUE(&queue, i);
	}

	MUNT_ASSERT(MQUE_IS_FULL(&queue));

	for (i = 1; i <= 5; i++) {
		int head;

		MUNT_ASSERT(!MQUE_IS_EMPTY(&queue));

		head = MQUE_HEAD(&queue);
		MUNT_ASSERT(head == i);

		MQUE_DEQUEUE(&queue);
	}

	MUNT_ASSERT(MQUE_IS_EMPTY(&queue));

	return 0;
}
