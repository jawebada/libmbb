libmbb - Debugging Macros
=========================

[*libmbb*](..) features a set of debugging macros defined in `mbb/debug.h`.

	#include "mbb/debug.h"

All debugging macros can be disabled by defining the macro NDEBUG before including
`mbb/debug.h`.

Have a look at [the example](../examples/debugging.c).

Print a string adding a newline
-------------------------------

	MDBG_PRINT_LN("hello, world!");

`2015-02-12T12:36:41.000 (examples/debugging.c, 11): hello, world!`

Print an integer (decimal)
--------------------------

	int i = 128;
	MDBG_PRINT_I(i);

`2015-02-12T12:36:41.000 (examples/debugging.c, 13): i = 128`

Print an integer (octal)
--------------------------

	int i = 128;
	MDBG_PRINT_O(i);

`2015-02-12T12:36:41.000 (examples/debugging.c, 14): i = O200`

Print an integer (hexadecimal)
--------------------------

	int i = 128;
	MDBG_PRINT_X(i);

`2015-02-12T12:36:41.000 (examples/debugging.c, 15): i = 0x80`

Print a character
-----------------

	char a = 'b';
	MDBG_PRINT_C(a);

`2015-02-12T12:36:41.000 (examples/debugging.c, 17): a = 'b'`

Print a float value
-------------------

	double pi = 3.14;
	MDBG_PRINT_F(pi);

`2015-02-12T12:36:41.000 (examples/debugging.c, 19): pi = 3.140000`

Print a string
--------------

	char foo[] = "bar";
	MDBG_PRINT_S(foo);

`2015-02-12T12:44:06.000 (examples/debugging.c, 23): foo: "bar"`

Print a memory region
---------------------

	uint8_t data[] = { 0xa, 0xb, 0xc, 0xd, 0xe, 0xf };
	MDBG_PRINT_MEM(data, sizeof(data));

`2015-02-12T12:36:41.000 (examples/debugging.c, 21): data(0xbf916bd9, 6): 0A 0B 0C 0D 0E 0F `

Generic printing macros
-----------------------

There are generic `printf`-like macros depending on the number of arguments.
This is because only few preprocessors feature support for variadic macros.

	MDBG_PRINT0(FORMAT)
	MDBG_PRINT1(FORMAT, ARG)
	MDBG_PRINT2(FORMAT, ARG1, ARG2)
	...
	MDBG_PRINT10(FORMAT, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10)

Example:

	int answer = 42;
	MDBG_PRINT1("the answer is %d\n", answer);

`2015-02-12T12:44:06.000 (examples/debugging.c, 27): the answer is 42`

Print errno
-----------

	read(55, NULL, 0);
	MDBG_PRINT_ERRNO("read");

`2015-02-12T13:13:41.000 (examples/debugging.c, 52): read: Bad file descriptor`

Assertions
----------

	int answer = 42;
	MDBG_ASSERT(answer == 41);

`2015-02-12T13:10:33.000 (examples/debugging.c, 54): assertion failed: answer == 41`

	debugging: examples/debugging.c:54: main: Assertion `answer == 41' failed.
	Aborted

Assert a line of code is never reached
--------------------------------------

	MDBG_NEVER_REACHED();

`2015-02-12T13:13:41.000 (examples/debugging.c, 56): assertion failed: !"reached"`

	debugging: examples/debugging.c:56: main: Assertion `!"reached"' failed.
	Aborted
