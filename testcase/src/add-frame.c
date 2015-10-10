#include "trap.h"

int add(int a, int b)
{
	if(!b)
		return a;
	return 1 + add(a, b - 1);
}

int main()
{
	int a = 4, b = 1000;
	add(a, b);

	return 0;
}
