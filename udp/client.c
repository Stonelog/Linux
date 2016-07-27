#include <stdio.h>
#include <sys/types.h>      
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/wait.h>
#include <pthread.h>

#define _SIZE_ 1024
#define _PORT_ 8080
#define _IP_ "127.0.0.1"

int main()
{
	int sock = socket(AF_INET,SOCK_DGRAM,0);
	if (sock < 0)
	{
		perror("socket");
		return 1;
	}
	
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(_PORT_);
	server.sin_addr.s_addr = inet_addr(_IP_);

	socklen_t addrlen = sizeof(server);

	char buf[_SIZE_];

	while(1)
	{
		memset(buf,'\0',sizeof(buf));
		printf("Please Enter:");
		fflush(stdout);
		int size = read(0,buf,sizeof(buf)-1);

		if (size > 0)
		{
			buf[size-1] = '\0';
			sendto(sock, buf, strlen(buf) ,0, (struct sockaddr*)&server, addrlen);

			if (strcmp(buf,"quit") == 0)
				break;

			memset(buf,'\0',sizeof(buf));
			ssize_t ret = recvfrom(sock, buf, sizeof(buf)-1, 0, (struct sockaddr *)&server, &addrlen);
			if(ret < 0)
			{
				perror("recvfrom");
			}
			printf("server :%s \n",buf);
			fflush(stdout);
		}
		else 
		{
			perror("read");
			return 3;
		}
	}

	close(sock);

	return 0;
}



