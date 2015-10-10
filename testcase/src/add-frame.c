#include "trap.h"

int a, b;

int add()
{
	if(!b)
		return a;
	b -= 1;
	return 1 + add();
}

int main()
{
	a = 4, b = 5;
	add();

	return 0;
}
