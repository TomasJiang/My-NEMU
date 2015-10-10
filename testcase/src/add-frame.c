#include "trap.h"

int add(int a, int b)
{
	set_bp();
	if(!b)
		return a;
	return 1 + add(a, b - 1);
}

int main()
{
	add(2, 3);
	add(3, 4);
	add(4, 5);

	HIT_GOOD_TRAP;

	return 0;
}
