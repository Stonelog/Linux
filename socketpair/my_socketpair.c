#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	int sv[2];
	int ret = socketpair(AF_LOCAL, SOCK_STREAM,0, sv);
	if(ret < 0)
	{
		perror("scoketpair");
		return 1;
	}

	pid_t pid = fork();
	if(pid < 0)
	{
		perror("fork");
		return 2;
	}
	else if(pid == 0)
	{
		close(sv[0]);
		const char *msg = "Hello Father,I am child";
		char buf[128];

		while(1)
		{
			write(sv[1], msg, strlen(msg));

			memset(buf,'\0',sizeof(buf));
			ssize_t size = read(sv[1],buf,sizeof(buf)-1);
			if(size > 0)
			{
				buf[size] = '\0';
				printf("[child] father -> child : %s \n",buf);
				fflush(stdout);
			}
		   sleep(1);
		}
	}
	else
	{
         close(sv[1]);
		 const char *msg = "Hello child, I am father";
		 char buf[128];

		 while(1)
		 {
			memset(buf,'\0',sizeof(buf));
			ssize_t size = read(sv[0],buf,sizeof(buf)-1);
			if(size > 0)
			{
				buf[size] = '\0';
				printf("[father] child -> father : %s \n",buf);
				fflush(stdout);
			}

			write(sv[0],msg,strlen(msg));
		 }
	}

	return 0;
}



