libmbb - Unit Tests
===================

*libmbb* features a simple unit test framework inspired by
[MinUnit](http://www.jera.com/techinfo/jtns/jtn002.html).


Writing unit tests
------------------

A test suite is a single C file containing any number of test cases. It must 

	#include "mbb/test.h"

A test case looks like this

	char *test_YOUR_NAME()
	{
		/* YOUR CODE */
		
		...

		MUNT_ASSERT(YOUR_ASSERTION);

		return 0;
	}

The test case function must return a string and its name must begin with
`test_`.

Have a look at *libmbb*'s own [tests](tests) as an example.

Compiling unit tests
--------------------

Each test suite is bundled to an executable using the `munt_main` tool.

Calling `munt_main test_suite.c > test_suite_main.c` pipes the automatically
generated main function to `test_suite_main.c`. This source code can be
compiled to an executable which exits returning `EXIT_SUCCESS` or
`EXIT_FAILURE` depending on whether the test suite failed.

