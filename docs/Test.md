libmbb - Unit Tests
===================

*libmbb* features a simple unit test framework inspired by
[MinUnit](http://www.jera.com/techinfo/jtns/jtn002.html).

Writing unit tests
------------------

A test suite is a single C file containing any number of test cases. It must 

	#include "mbb/test.h"

A test case looks like this:

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

The test case function must return a string and its name must begin with
`test_`. It must return 0 if no error occured. The `MUNT_ASSERT(EXPRESSION)`
macro is used for the actual test assertions. It makes the test case function
return a string if `EXPRESSION` is false.

Have a look at *libmbb*'s own [tests](../tests) as an example.

Compiling unit tests
--------------------

The `munt_main` tool is used to generate a main function for a test suite,
printing it to `stdout`. Calling `munt_main test_suite.c > test_suite_main.c`
pipes the automatically generated main function to `test_suite_main.c`. 

This source code can be compiled to an executable which exits returning
`EXIT_SUCCESS` or `EXIT_FAILURE` depending on whether the test suite failed.

Running unit tests
------------------

The output of the automatically generated test program looks like this:

	tests/test_queue
	
	Test suite 'test_queue':
	
	### test_enqueue_dequeue ###
	Assertion passed (013): MQUE_IS_EMPTY(&queue)
	Assertion passed (016): !MQUE_IS_FULL(&queue)
	Assertion passed (016): !MQUE_IS_FULL(&queue)
	Assertion passed (016): !MQUE_IS_FULL(&queue)
	Assertion passed (016): !MQUE_IS_FULL(&queue)
	Assertion passed (016): !MQUE_IS_FULL(&queue)
	Assertion passed (023): !MQUE_IS_EMPTY(&queue)
	Assertion passed (023): !MQUE_IS_EMPTY(&queue)
	Assertion passed (023): !MQUE_IS_EMPTY(&queue)
	Assertion passed (023): !MQUE_IS_EMPTY(&queue)
	Assertion passed (023): !MQUE_IS_EMPTY(&queue)
	Assertion passed (029): MQUE_IS_EMPTY(&queue)
	### passed ###
	
	Test suite 'test_queue' passed (1 test run)

