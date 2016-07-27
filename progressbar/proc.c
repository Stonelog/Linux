#include "proc.h"

void Proc()
{
	char Buf[102];
	int i = 0;
	memset(Buf,'\0', sizeof(Buf));
	char ch[]="|/_\\";

	while( i < 101)
	{

		printf("[%-101s] [%d%%] [%c]\r" , Buf , i, ch[i%4] );
        fflush(stdout);

		Buf[i] = '=';
		++i;

		usleep(60000);
	}

	printf("\n");
}


