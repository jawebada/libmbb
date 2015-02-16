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

#define MUNT_RUN_TEST(test) do { \
	char *message; \
	fprintf(stderr, "### " #test " ###\n"); \
	message = test(); \
	munt_tests_run++; \
	if (message) { \
		MUNT_COLOR(fprintf(stderr, "### " #test " FAILED: '%s' ###\n\n", message)); \
		return message; \
	} else { \
		fprintf(stderr, "### passed ###\n\n"); \
	} \
} while (0)

#endif /* MBB_TEST_H */
