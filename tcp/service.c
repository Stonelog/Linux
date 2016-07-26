#include <stdio.h>
#include <sys/types.h>      
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/wait.h>
#include <pthread.h>

//const int _SIZE_ = 1024;
#define  _SIZE_  1024
#define _PORT_ 8080
#define _IP_ "127.0.0.1"

void *thread_run(void *arg)
{
	int fd = (int)arg;

	char buf[_SIZE_];

	int done = 0;
	while(!done)
	{
		memset(buf,'\0',sizeof(buf));
	    int size = read(fd,buf, sizeof(buf)-1);
		if(size > 0)
		{
			buf[size-1]='\0';
			printf("client : %s\n",buf);
			fflush(stdout);
			write(fd,buf,size);
		}
		else if (size == 0)
		{
			printf("client close... read done....\n");
			return NULL;
		}
		else
		{
			perror("read");
			return NULL;
		}
	}

	return (void *)0;
}

int main()
{
	int  listen_sock = socket(AF_INET,SOCK_STREAM,0); // IPV4 , 流式套接字 
	if(listen_sock < 0)
	{
		perror("socket");
	}

	int opt = 1;
	socklen_t optlen = sizeof(opt);
	setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, optlen);
	
	struct sockaddr_in local;
	local.sin_family = AF_INET;       
	local.sin_port = htons(_PORT_);   //主机字节序列向网络字节序列转换
	local.sin_addr.s_addr =inet_addr(_IP_);

	socklen_t addrlen = sizeof(local);

	if( bind(listen_sock, (struct sockaddr *)&local, addrlen) < 0)
	{
		perror("bind");
		close(listen_sock);
		return 1;
	}

	if( listen(listen_sock, 4) < 0)  //当前连接队列的长度
	{
		perror("listen");
		close(listen_sock);
		return 2;
	}

	int done = 0;

	while(!done)
	{
		struct sockaddr_in client;
		socklen_t client_len = 0;
		int client_sock = accept(listen_sock,(struct sockaddr*)&client,&client_len); //两个输出型参数
		if(client_sock < 0)
		{
			perror("accept");
			close(listen_sock);
			return 3;
		}

		printf("create new client :[ip]:%s [port]:%d \n",inet_ntoa(client.sin_addr), ntohs(client.sin_port));

		pthread_t th1;
		pthread_create(&th1,NULL,thread_run ,(void *)client_sock);
		pthread_detach(th1);
	
	//	pid_t pid = fork();
	//	if (pid == 0)
	//	{
	//		char buf[_SIZE_];
	//		while(1)
	//		{
	//			memset(buf,'\0',sizeof(buf));
	//		    int size = read(client_sock ,buf, sizeof(buf)-1);
	//			if(size > 0)
	//			{
	//				buf[size-1]='\0';
	//			}
	//			printf("client : %s\n",buf);
	//			fflush(stdout);
	//			write(client_sock,buf,size);
	//	
	//		//	memset(buf,'\0',sizeof(buf));
	//		//	printf("service : ");
	//		//	fflush(stdout);
	//	
	//		//	int size = read(1,buf,sizeof(buf));
	//		//	if(size < 0)
	//		//	{
	//		//		perror("read");
	//		//		return 4;
	//		//	}
	//		//	buf[size-1] = '\0';
	//			//buf[strlen(buf)-1] = '\0';
	//			//write(client_sock,buf,strlen(buf));
	//		
	//	    //	write(client_sock,buf,size);
	//		}
	//	}
	//	else
	//	{
	//		waitpid(-1,NULL,WNOHANG);
	//	}
	}

	close(listen_sock);

	return 0;
}


