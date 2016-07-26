#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/unistd.h>
#include <sys/wait.h>

int main()
{
	pid_t pid;
	pid = fork();

	if (pid < 0 )
	{
		printf("Create Child fail\n");
		return 1;
	}
	
	if (pid == 0)
	{
		printf("Child PID: %d ;Child runing...\n",getpid());
		sleep(5);

		exit(275);
	}
	else
	{
		int status = 0;
		pid_t ret = 0;

		ret = waitpid(-1, &status, 0);
		printf("Father PID : %d ,Father runing.....\n",getpid());

		if (WIFEXITED(status) && ret == pid)
		{
			printf("wait 5s child successed , chils return code is: %d \n", WEXITSTATUS(status));
		}
		else
		{
			printf("Wait Failed \n");
			return 1;
		}

		return 0;
	}

	return 0;
}
