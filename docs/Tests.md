libmbb - Unit Tests
===================

*libmbb* features a simple unit test framework inspired by
[MinUnit](http://www.jera.com/techinfo/jtns/jtn002.html).

Writing unit tests
------------------

A test suite is a single C file containing any number of test cases. It must 

	#include "mbb/test.h"

A test case looks like this:

	char *test_YOUR_NAME()
	{
		/* YOUR CODE */
		
		...

		MUNT_ASSERT(YOUR_ASSERTION);

		return 0;
	}

The test case function must return a string and its name must begin with
`test_`. It must return 0 if no error occured. The `MUNT_ASSERT(EXPRESSION)`
macro makes the test case function return a string if `EXPRESSION` is false.

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

