#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void my_daemon()
{
	pid_t pid = fork();

	if (pid > 0)
	{
		exit(1) ;
	}

	setsid();

	if (chdir("/") < 0 )
	{
		perror("chdir");
		return ;
	}

	close(0);
	close(1);
	close(2);

	signal(SIGCHLD,SIG_IGN);
}

int main()
{
	printf("my_daemon start...\n");

	my_daemon();
	//daemon(0,0);

	while(1);

	return 0;
}


