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

void print(const char *argv)
{
	printf("Usage: %s [IP] [PORT] \n",argv);
}

int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		print(argv[0]);
		return -1;
	}


	int sock = socket(AF_INET,SOCK_DGRAM,0);
	if (sock < 0)
	{
		perror("socket");
		return 1;
	}
	
	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(atoi(argv[2]));
	local.sin_addr.s_addr = inet_addr(argv[1]);

	socklen_t addrlen = sizeof(local);

	if (bind(sock,(struct sockaddr *)&local,addrlen) <  0)
	{
		perror("bind");
		return 2;
	}

	struct sockaddr_in client;
	socklen_t len = sizeof(client);

	char buf[_SIZE_];

	while(1)
	{
		memset(buf,'\0',sizeof(buf));
	
		ssize_t ret = recvfrom(sock, buf, sizeof(buf)-1, 0, (struct sockaddr *)&client, &len);
		if (ret > 0)
		{
			printf("client :%s \n",buf);
			fflush(stdout);
	
			sendto( sock, buf, strlen(buf) , 0, (struct sockaddr*)&client , len );
		}
		else 
		{
			perror("recvfrom");
			return 3;
		}
	}

	close(sock);

	return 0;
}


