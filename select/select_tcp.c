#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>         
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>

static int startup(const char *_ip_ ,int _port_)
{
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock < 0)
	{
		perror("socket");
		exit(2);
	}

	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(_port_);
	local.sin_addr.s_addr = inet_addr(_ip_); 

	int opt = 1;
	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));	

	if(bind(sock,(struct sockaddr *)&local,sizeof(local)) < 0)
	{
		perror("bind");
		exit(3);
	}
	if(listen(sock,4) < 0)
	{
		perror("listen");
		exit(4);
	}

	return sock;
}

static void add_fd(int *_fd, int _fd_nums, int fd)
{
	if(NULL == _fd || _fd_nums <= 0 || fd < 0)
		return ;

	int index = 0;
	for(index = 0; index < _fd_nums; ++index)
	{
		if(_fd[index] == -1)
		{
			_fd[index] = fd;
			break;
		}
	}
	if(index == _fd_nums)
	{
		exit(7);
	}
}

static load_set(int *_fd, int _fd_nums, int *max_fd,fd_set *set)
{
	*max_fd = -1;
	FD_ZERO(set);
	
	int index = 0;
	for (index = 0; index < _fd_nums ; ++index)
	{
		if(_fd[index] != -1)
		{
			FD_SET(_fd[index],set);
			(*max_fd) = (*max_fd) < _fd[index] ? _fd[index] : (*max_fd);
		}
	}
}

static void Usage(const char *proc)
{
	printf("Usage : %s [IP] [PORT]\n",proc);
}

int fd[128];
int fd_nums = sizeof(fd)/sizeof(fd[0]);

int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		Usage(argv[0]);
		exit(1);
	}

	int listen_sock = startup(argv[1],atoi(argv[2]));

	int i = 0;
	for(i = 0; i < fd_nums ; ++i)
	{
		fd[i] = -1;
	}
	fd[0] = listen_sock;

	fd_set rset;  //可读文件描述符集
	int max_fd = -1;

	int done = 0;
	while(!done)
	{
		//struct timeval timeout = {5,0};

		load_set(fd, fd_nums, &max_fd, &rset);

		switch(select(max_fd + 1,&rset,NULL,NULL,NULL))
		{
			case -1:
				perror("select"); exit(5); break;
			case 0 :
				printf("timeout...\n"); break;
			default:

				for (i = 0; i < fd_nums; ++i)
				{
					//如果是主socket的事件，则表示有新的连接
					if(fd[0] == listen_sock && FD_ISSET(listen_sock,&rset))
					{
						struct sockaddr_in peer;
						socklen_t len = 0;

						int new_fd = accept(listen_sock,(struct sockaddr *)&peer,&len);
						if(new_fd <  0)
						{
							perror("accept");
							exit(6);
						}

						printf("create new client : ip:[%s] -> port:[%d]\n",inet_ntoa(peer.sin_addr),ntohs(peer.sin_port));
					
						add_fd(fd,fd_nums,new_fd);
						break;
					}
					else if(fd[i] != -1 && FD_ISSET(fd[i],&rset))
					{
						char buf[1024];
						memset(buf,'\0',sizeof(buf));

						ssize_t size = read(fd[i],buf,sizeof(buf)-1);
						if(size > 0)
						{
							buf[size-1] = '\0';
							printf("client : %s\n",buf);
						}
						else if(size == 0)
						{
							printf("client done .... \n",buf);
							close(fd[i]);
							fd[i] = -1;
						}
						else
						{
							perror("read");
							exit(8);
						}
					}
					else
					{  }
				}
					
				break;
		}
	}

	return 0;
}

// int select(int nfds, fd_set *readfds, fd_set *writefds,fd_set *exceptfds, struct timeval *timeout);
// void FD_CLR(int fd, fd_set *set);
// int  FD_ISSET(int fd, fd_set *set);
// void FD_SET(int fd, fd_set *set);
// void FD_ZERO(fd_set *set);


//int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);


