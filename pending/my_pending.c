#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void print_pend(sigset_t *pending)
{
	size_t i = 0;
	for ( ; i < 32 ; ++i)
	{
		if(sigismember(pending,i))
		{
			putchar('1');
		}
		else
		{
			putchar('0');
		}
	}
	printf("\n");
}

int main()
{
	sigset_t block , out , pending;

	sigemptyset(&block);
	sigemptyset(&out);
	sigemptyset(&pending);

	sigaddset(&block,SIGINT);
	sigprocmask(SIG_SETMASK,&block,&out);

	while(1)
	{
		sigpending(&pending);
		print_pend(&pending);
		sleep(1);
	}

	return 0;
}



