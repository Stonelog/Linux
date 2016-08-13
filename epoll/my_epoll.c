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
#include <fcntl.h>
#include <sys/epoll.h>

#define _SIZE_ 1024

static int startup(char *_ip, int _port)
{
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock < 0)
	{
		perror("socket");
		exit(2);
	}

	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(_port);
	local.sin_addr.s_addr = inet_addr(_ip);

	//avoid two Maximum Segment Lifetime (2MSL)
	int opt = 1;
	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));	

	if(bind(sock,(struct sockaddr *)&local,sizeof(local)) < 0)
	{
		perror("bind");
		exit(3);
	}

	if(listen(sock,5) < 0)
	{
		perror("listen");
		exit(4);
	}

	return sock;
}

static void Usage(const char *proc)
{
	printf("Usage : %s [IP] [PORT] \n",proc);
}

int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		Usage(argv[0]);
		exit(1);
	}

	int listen_sock = startup(argv[1],atoi(argv[2]));

	int epfd = epoll_create(5);
	struct epoll_event ev,events[20];

	ev.events = EPOLLIN;
	ev.data.fd = listen_sock;

	fcntl(listen_sock, F_SETFL, O_NONBLOCK );

    epoll_ctl(epfd, EPOLL_CTL_ADD, listen_sock, &ev); //将新的fd添加到epoll的监听队列中

	int i = 0;
    int nfds = 0;

	while(1)
	{
		nfds = epoll_wait(epfd, events, sizeof(events)/sizeof(events[0]), -1); 

		switch (nfds)
		{
			case -1:
				perror("epoll_wait");  break;
			case 0:
				printf("timeout....\n");  break;
			default:

				for(i = 0; i < nfds; ++i)
    		    {
					int sockfd =  events[i].data.fd;

    		        if(events[i].data.fd == listen_sock) //如果是主socket的事件，则表示有新的连接
    		        {
						struct sockaddr_in clientaddr;
						socklen_t clilen = sizeof(clientaddr);

    		            int connfd = accept(listen_sock,(struct sockaddr *)&clientaddr, &clilen); //accept这个连接
						if (connfd < 0)
						{
							perror("accept");
							continue;
						}
				
						printf("create new client , [%s]->[%d] \n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));

						fcntl(connfd, F_SETFL, O_NONBLOCK );

    		            ev.data.fd = connfd;
    		            ev.events = EPOLLIN | EPOLLET;
    		            epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&ev); //将新的fd添加到epoll的监听队列中
    		        }
				    else if( events[i].events & EPOLLIN ) //接收到数据，读socket
				    {
						if (sockfd  < 0) 
							continue;

						char buf[_SIZE_];
						memset(buf,'\0',sizeof(buf));
						ssize_t size = read(sockfd, buf, sizeof(buf)-1);    //读
						if(size > 0)
						{
							buf[size-1] = '\0';
							printf(" client : %s \n",buf);
							fflush(stdout);

							ev.data.fd = sockfd;
							ev.events = EPOLLOUT | EPOLLET;
							epoll_ctl(epfd,EPOLL_CTL_MOD,sockfd,&ev);//修改标识符，等待下一个循环时发送数据，异步处理的精髓
						}
						else if(size == 0)
						{
							printf("client done....\n");
						}
						else
						{
							perror("read");
						}
					}
					else if(events[i].events & EPOLLOUT) //有数据待发送，写socket
					{
					//	char buf1[] = "HTTP/1.1 200 OK\r\nContent-Length:128\r\n\r\n \
					//				    <html><head><title>Http</title></head> \
					//	  				<body><h1> hello world </h1></body></html>";

					//	send(sockfd, buf1, sizeof(buf1), 0 );        //发送数据
					//	sleep(2);
					//	close(sockfd);

						char buf[] = "echo: hello world\n";
						send(sockfd, buf, sizeof(buf), 0 );        //发送数据

						ev.data.fd = sockfd;
						ev.events = EPOLLIN | EPOLLET;
						epoll_ctl(epfd,EPOLL_CTL_MOD,sockfd,&ev); //修改标识符，等待下一个循环时接收数据
					}
					else
					{
				                 //其他情况的处理
					}

					break;
				}
		}
	}

	close(epfd);

	return 0;
}


