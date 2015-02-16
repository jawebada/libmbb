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

	for (i = 1; i <= 5; i++) {
		int head;

		MUNT_ASSERT(!MQUE_IS_EMPTY(&queue));

		head = MQUE_HEAD(&queue);
		MQUE_DEQUEUE(&queue);;

		MUNT_ASSERT(head == i);
	}

	MUNT_ASSERT(MQUE_IS_EMPTY(&queue));

	return 0;
}
