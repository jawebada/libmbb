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

#ifndef MBB_TEST_H
#define MBB_TEST_H

#include <stdio.h>
#include <stdlib.h>

#if __linux
/*
 * vt100-compatible text colors
 * http://linuxgazette.net/issue65/padala.html
 */
void textcolor(int attr, int fg, int bg);

#define MUNT_RESET	0
#define MUNT_BRIGHT	1
#define MUNT_BLACK	0
#define MUNT_RED	1
#define MUNT_WHITE	7

#define MUNT_COLOR(statement) do { \
	textcolor(MUNT_BRIGHT, MUNT_RED, MUNT_BLACK); \
	statement; \
	textcolor(MUNT_RESET, MUNT_WHITE, MUNT_BLACK); \
} while (0) 
#else
#define MUNT_COLOR(statement) do { \
	statement; \
} while (0)
#endif /* __linux */

/* 
 * Minunit-like test 'framework'
 * http://www.jera.com/techinfo/jtns/jtn002.html
 */
#define MUNT_ASSERT(TEST) do { \
	static char msgstr[256]; \
	snprintf(msgstr, sizeof(msgstr), "Assertion FAILED (%03d): %s", __LINE__, #TEST); \
	if (!(TEST)) return msgstr; \
	fprintf(stderr, "Assertion passed (%03d): %s\n", __LINE__, #TEST); \
} while (0)

int munt_tests_run;

#define MUNT_RUN_TEST(TEST) do { \
	char *message; \
	fprintf(stderr, "### " #TEST " ###\n"); \
	message = TEST(); \
	munt_tests_run++; \
	if (message) { \
		MUNT_COLOR(fprintf(stderr, "### " #TEST " FAILED: '%s' ###\n\n", message)); \
		return message; \
	} else { \
		fprintf(stderr, "### passed ###\n\n"); \
	} \
} while (0)

#endif /* MBB_TEST_H */
