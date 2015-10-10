#include "trap.h"

int add(int a, int b)
{
	if(!b)
		return a;
	return 1 + add(a, b - 1);
}

int main()
{
	add(4, 1000);

	return 0;
}
