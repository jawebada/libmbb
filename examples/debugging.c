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

#include "mbb/debug.h"
#include "mbb/types.h"
#include <unistd.h>

int main(void)
{
	int i = 128;
	double pi = 3.14;
	char a = 'b';
	char foo[] = "bar";
	uint8_t data[] = { 0xa, 0xb, 0xc, 0xd, 0xe, 0xf };
	int answer = 42;

	MDBG_PRINT_LN("hello, world!");

	MDBG_PRINT_I(i);
	MDBG_PRINT_O(i);
	MDBG_PRINT_X(i);

	MDBG_PRINT_C(a);

	MDBG_PRINT_F(pi);

	MDBG_PRINT_S(foo);

	MDBG_PRINT_MEM(data, sizeof(data));

	MDBG_PRINT1("the answer is %d\n", answer);

	/* expected to fail */
	read(55, NULL, 0);
	MDBG_PRINT_ERRNO("read");

	MDBG_ASSERT(answer == 42);

	MDBG_NEVER_REACHED();

	return 0;
}
