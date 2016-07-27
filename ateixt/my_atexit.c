#include <stdio.h>
#include <stdlib.h>

void fun1()
{
	printf("This Fun1 \n");
}

void fun2()
{
	printf("This Fun2 \n");
}

void fun3()
{
	printf("This Fun3 \n");
}
 
void fun4()
{
	printf("This Fun4 \n");
}

int main()
{
	atexit(fun1);
	atexit(fun2);
	atexit(fun3);
	atexit(fun4);

	sleep(4);
	return 0;
}



