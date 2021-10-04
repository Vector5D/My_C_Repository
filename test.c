#include <stdio.h>

int add(int x,int y)
{
	return x + y;
}

int main()
{
	int x = 10;
	int y = 10;

	printf("x + y = %d\n",add(x,y));
	return 0;
}
