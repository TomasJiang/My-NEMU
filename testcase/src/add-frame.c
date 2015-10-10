#include "trap.h"

int a, b;

int add()
{
	if(!b)
		return a;
	return 1 + add(a, b - 1);
}

int main()
{
	a = 4, b = 5;
	add();

	HIT_GOOD_TRAP;

	return 0;
}
