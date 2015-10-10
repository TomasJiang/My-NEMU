#include "trap.h"

int add(int a, int b)
{
	if(!b)
		return a;
	return 2 + add(a - 1, b - 1);
}

int main()
{
	int a = 256, b = 1000;
	add(a, b);

	return 0;
}
