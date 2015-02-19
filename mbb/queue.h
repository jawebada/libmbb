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

#ifndef MBB_QUEUE_H
#define MBB_QUEUE_H

#include "types.h"

/* 
 * Simple type-safe fixed-capacity queues.
 *
 * Always check MQUE_LENGTH(), MQUE_IS_EMPTY(), or MQUE_IS_FULL(),
 * respectively, before calling MQUE_ENQUEUE() or MQUE_DEQUEUE().
 * MQUE_ENQUEUE() will do nothing if the queue is full.  MQUE_DEQUEUE() will do
 * nothing if the queue is empty.
 *
 * Example:
 *
 * {
 * 	MQUE_DEFINE_STRUCT(int, 5) event_queue;
 *
 * 	MQUE_INITIALISE(&event_queue);
 *
 *	MQUE_ENQUEUE(&event_queue, 4);
 *	MQUE_ENQUEUE(&event_queue, 3);
 *	MQUE_ENQUEUE(&event_queue, 2);
 *
 * 	while (MQUE_LENGTH(&event_queue)) {
 *		printf("event: %d\n", MQUE_HEAD(&event_queue));
 * 		MQUE_DEQUEUE(&event_queue);
 *	}
 * }
 *
 * Output:
 *
 * event: 4
 * event: 3
 * event: 2
 */

#define MQUE_DEFINE_STRUCT(TYPE, CAPACITY) \
struct { \
	int first; \
	int last; \
	size_t count; \
	TYPE data[CAPACITY]; \
}

#define MQUE_CAPACITY(Q) (sizeof((Q)->data) / sizeof((Q)->data[0]))

#define MQUE_INITIALISER { 0, -1, 0 }

#define MQUE_INITIALISE(Q) do { \
	(Q)->first = 0; \
	(Q)->last = -1; \
	(Q)->count = 0; \
} while (0)

#define MQUE_LENGTH(Q) (Q)->count

#define MQUE_IS_FULL(Q) (MQUE_LENGTH(Q) == MQUE_CAPACITY(Q))

#define MQUE_IS_EMPTY(Q) (MQUE_LENGTH(Q) == 0)

#define MQUE_ENQUEUE(Q, ELEMENT) do { \
	if (MQUE_IS_FULL(Q)) break; \
	(Q)->last = ((Q)->last + 1) % MQUE_CAPACITY(Q); \
	(Q)->data[(Q)->last] = ELEMENT; \
	(Q)->count += 1; \
} while (0)

#define MQUE_HEAD(Q) ((Q)->data[(Q)->first])

#define MQUE_DEQUEUE(Q) do { \
	if (MQUE_IS_EMPTY(Q)) break; \
	(Q)->first = ((Q)->first + 1) % MQUE_CAPACITY(Q); \
	(Q)->count -= 1; \
} while (0)

#endif /* MBB_QUEUE_H */
