#include "mbb/debug.h"
#include "mbb/types.h"

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

	return 0;
}
