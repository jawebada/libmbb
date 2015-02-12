#ifndef MBB_DEBUG_H
#define MBB_DEBUG_H

#ifndef NDEBUG

# include <assert.h>

# ifndef MDBG_PRINTF
int mdbg_printf(const char *format, ...);
#  define MDBG_PRINTF mdbg_printf
# endif

/* 
 * MDBG_TIMESTAMP should name a function which writes a timestamp in ISO 8601
 * format to a string: 2015-02-10T13:22:35.102
 */
# ifndef MDBG_TIMESTAMP
int mdbg_timestamp(char *out, int size);
#  define MDBG_TIMESTAMP mdbg_timestamp
# endif

# define MDBG_PRINT_PREFIX() do { \
	char timestamp[24]; \
	if (MDBG_TIMESTAMP(timestamp, sizeof(timestamp)) != 0) break; \
	MDBG_PRINTF("%s (%s, %d): ", timestamp, __FILE__, __LINE__); \
} while (0)

# define MDBG_PRINT0(STR) do { \
	MDBG_PRINT_PREFIX(); \
	MDBG_PRINTF(STR); \
} while (0)

# define MDBG_PRINT1(STR, ARG) do { \
	MDBG_PRINT_PREFIX(); \
	MDBG_PRINTF(STR, ARG); \
} while (0)

# define MDBG_PRINT2(STR, ARG1, ARG2) do { \
	MDBG_PRINT_PREFIX(); \
	MDBG_PRINTF(STR, ARG1, ARG2); \
} while (0)

# define MDBG_PRINT3(STR, ARG1, ARG2, ARG3) do { \
	MDBG_PRINT_PREFIX(); \
	MDBG_PRINTF(STR, ARG1, ARG2, ARG3); \
} while (0)

# define MDBG_PRINT4(STR, ARG1, ARG2, ARG3, ARG4) do { \
	MDBG_PRINT_PREFIX(); \
	MDBG_PRINTF(STR, ARG1, ARG2, ARG3, ARG4); \
} while (0)

# define MDBG_PRINT5(STR, ARG1, ARG2, ARG3, ARG4, ARG5) do { \
	MDBG_PRINT_PREFIX(); \
	MDBG_PRINTF(STR, ARG1, ARG2, ARG3, ARG4, ARG5); \
} while (0)

# define MDBG_PRINT6(STR, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6) do { \
	MDBG_PRINT_PREFIX(); \
	MDBG_PRINTF(STR, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6); \
} while (0)

# define MDBG_PRINT7(STR, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7) do { \
	MDBG_PRINT_PREFIX(); \
	MDBG_PRINTF(STR, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7); \
} while (0)

# define MDBG_PRINT8(STR, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8) do { \
	MDBG_PRINT_PREFIX(); \
	MDBG_PRINTF(STR, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8); \
} while (0)

# define MDBG_PRINT9(STR, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9) do { \
	MDBG_PRINT_PREFIX(); \
	MDBG_PRINTF(STR, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9); \
} while (0)

# define MDBG_PRINT10(STR, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10) do { \
	MDBG_PRINT_PREFIX(); \
	MDBG_PRINTF(STR, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10); \
} while (0)

# define MDBG_PRINT_LN(LINE) MDBG_PRINT1("%s\n", LINE)
# define MDBG_PRINT_I(INTVAL) MDBG_PRINT1(#INTVAL " = %i\n", (int) (INTVAL))
# define MDBG_PRINT_O(OCTVAL) MDBG_PRINT1(#OCTVAL " = O%o\n", (unsigned int) (OCTVAL))
# define MDBG_PRINT_X(HEXVAL) MDBG_PRINT1(#HEXVAL " = 0x%02X\n", (unsigned int) (HEXVAL))
# define MDBG_PRINT_C(CHARVAL) MDBG_PRINT1(#CHARVAL " = '%c'\n", (int) (CHARVAL))
# define MDBG_PRINT_F(FLOATVAL) MDBG_PRINT1(#FLOATVAL " = %f\n", (double) (FLOATVAL))
# define MDBG_PRINT_S(STRVAL) MDBG_PRINT1(#STRVAL ": \"%s\"\n", (STRVAL))
# define MDBG_PRINT_P(MEMADDR) MDBG_PRINT1(#MEMADDR ": %p\n", (MEMADDR))

# define MDBG_PRINT_ERRNO(MSG) MDBG_PRINT1(MSG ": %s\n", strerror(errno))

# define MDBG_PRINT_MEM(PTR, SIZE) do { \
	int i; \
	MDBG_PRINT_PREFIX(); \
	MDBG_PRINTF("%s(%p, %d): ", #PTR, PTR, SIZE); \
	for (i = 0; i < SIZE; i++) \
		MDBG_PRINTF("%02X ", PTR[i]); \
	MDBG_PRINTF("\n"); \
} while (0)

# define MDBG_ASSERT(EXP) do { \
	if (!(EXP)) { \
		MDBG_PRINT1("assertion failed: %s\n", #EXP); \
		assert(EXP); \
	} \
} while (0)

# define MDBG_NEVER_REACHED() MDBG_ASSERT(!"reached")

#else /* NDEBUG */

# define MDBG_PRINT0(STR) do {} while(0)
# define MDBG_PRINT1(STR, ARG) do {} while(0)
# define MDBG_PRINT2(STR, ARG1, ARG2) do {} while(0)
# define MDBG_PRINT3(STR, ARG1, ARG2, ARG3) do {} while(0)
# define MDBG_PRINT4(STR, ARG1, ARG2, ARG3, ARG4) do {} while(0)
# define MDBG_PRINT5(STR, ARG1, ARG2, ARG3, ARG4, ARG5) do {} while(0)
# define MDBG_PRINT6(STR, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6) do {} while(0)
# define MDBG_PRINT7(STR, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7) do {} while(0)
# define MDBG_PRINT8(STR, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8) do {} while(0)
# define MDBG_PRINT9(STR, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9) do {} while(0)
# define MDBG_PRINT10(STR, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10) do {} while(0)

# define MDBG_PRINT_LN(LINE)do {} while(0)
# define MDBG_PRINT_I(INTVAL) do {} while(0)
# define MDBG_PRINT_O(OCTVAL) do {} while(0)
# define MDBG_PRINT_X(HEXVAL) do {} while(0)
# define MDBG_PRINT_C(CHARVAl)do {} while(0)
# define MDBG_PRINT_F(FLOATVal) do {} while(0)
# define MDBG_PRINT_S(STRVAL) do {} while(0)
# define MDBG_PRINT_P(MEMVAL)do {} while(0)

# define MDBG_PRINT_ERRNO(MSG)do {} while(0)
# define MDBG_PRINT_MEM(PTR, SIZE)do {} while(0)
# define MDBG_ASSERT(EXP) do {} while(0)
# define MDBG_NEVER_REACHED()do {} while(0)

#endif /* NDEBUG */

#endif /* MBB_DEBUG_H */
