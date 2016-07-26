#include <stdio.h>
#include <sys/types.h>      
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define _SIZE_  1024
#define _PORT_ 8080
#define _IP_ "127.0.0.1"

int main()
{
	int  server_sock = socket(AF_INET,SOCK_STREAM,0); // IPV4 , 流式套接字 
	if(server_sock < 0)
	{
		perror("socket");
	}

	struct sockaddr_in local;
	local.sin_family = AF_INET;       
	local.sin_port = htons(_PORT_);   //主机字节序列向网络字节序列转换
	local.sin_addr.s_addr =inet_addr(_IP_);

	socklen_t addrlen = sizeof(local);

	if(connect(server_sock, (struct sockaddr *)&local, addrlen) < 0)
	{
		perror("connect");
		close(server_sock);
		return 1;
	}

	char buf[_SIZE_];
	int done = 0;

	while(!done)
	{
		memset(buf,'\0',sizeof(buf));
		printf("Please Enter :");
		fflush(stdout);
		int size = read(0,buf,sizeof(buf)-1);
		if(size < 0)
		{
			perror("read");
			return 2;
		}
		buf[size-1] = '\0';
		write(server_sock,buf,size);
		if(strcmp(buf,"quit") == 0)
		{
			break;
		}
		
		memset(buf,'\0',sizeof(buf));
		read(server_sock,buf,sizeof(buf));
		printf("server : %s\n",buf);
		fflush(stdout);
	}

	close(server_sock);

	return 0;
}


