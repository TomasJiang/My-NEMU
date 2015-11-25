#include "trap.h"

int add(int a, int b)
{
	if(!b)
		return a;
	return 2 + add(a - 1, b - 1);
}

int main()
{
    int a = 3, b = 2;
	//int a = 256, b = 256;
	int c = add(a, b);

    nemu_assert(c == a+b);
    HIT_GOOD_TRAP;


	return 0;
}
