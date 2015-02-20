libmbb - Fixed-capacity Queues
==============================

[*libmbb*](..) features a simple set of macros to implement type-safe
fixed-capacity queues.

The macros are defined in `mbb/queue.h`. 

	#include "mbb/queue.h"

Example
-------

	{
		MQUE_DEFINE_STRUCT(int, 5) event_queue;
	
		MQUE_INITIALISE(&event_queue);
	
		MQUE_ENQUEUE(&event_queue, 4);
		MQUE_ENQUEUE(&event_queue, 3);
		MQUE_ENQUEUE(&event_queue, 2);
	
		while (MQUE_LENGTH(&event_queue)) {
			printf("event: %d\n", MQUE_HEAD(&event_queue));
			MQUE_DEQUEUE(&event_queue);
		}
	}

Output:

	event: 4
	event: 3
	event: 2

Macros
------

For the following macros `Q` refers to a pointer to a queue structure defined
by `MQUE_DEFINE_STRUCT`. Note that these macros cannot be used on pointers
passed to functions, the queue structure must be 'in scope'.

	MQUE_DEFINE_STRUCT(TYPE, CAPACITY) [ = MQUE_INITIALISER];

Defines a queue of CAPACITY TYPEs. You can assign `MQUE_INITIALISER` to
initialise the queue without calling `MQUE_INITIALISE`.

	MQUE_INITIALISE(Q);

Initialise a queue.

	MQUE_CAPACITY(Q)

Returns the queue's capacity.

	MQUE_LENGTH(Q)

Returns the queue's length.

	MQUE_IS_FULL(Q)

Returns true if the queue is full, false otherwise.

	MQUE_IS_EMPTY(Q)

Returns true if the queue is empty, false otherwise.

	MQUE_ENQUEUE(Q, ELEMENT);

Enqueue an element. `ELEMENT` is assigned to the enqueued element. Always check
whether the queue is full before calling `MQUE_ENQUEUE`. It will do nothing if
the queue is full.

	MQUE_HEAD(Q)

Returns the queue's head, which is of type `TYPE`.

	MQUE_DEQUEUE(Q);

Dequeue the queue's head. `MQUE_DEQUEUE` will do nothing if the queue is empty.

This is *not* a function returning the head. Use `MQUE_HEAD` instead. Refer to
the example above. 

