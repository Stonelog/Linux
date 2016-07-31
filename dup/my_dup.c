#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main()
{
	int fd = open("./log",O_CREAT | O_WRONLY,0644);
	if(fd < 0)
	{	
		perror("open");
		return 1;
	}

//	close(1);
//	int new_fd = dup(fd); //从最小的没有被使用的文件描述符开始分配
//	if(new_fd < 0)
//	{
//		perror("dup");
//		return 2;
//	}

	int new_fd = dup2(fd, 1); // 1.close(1)  2.将fd位置的文件地址拷贝到1的位置上
	if(new_fd < 0)
	{
		perror("dup2");
		return 3;
	}

//	可以证明文件描述符fd(3)没有被关闭
//	char a[] = "1234567\n";
//	write(fd,a,sizeof(a));

	printf("new_fd :[%d], old_fd:[%d] \n",new_fd,fd);

	char buf[1024];
	int done = 0;

	while(!done)
	{
		memset(buf, '\0',sizeof(buf));

		ssize_t size = read(0, buf, sizeof(buf)-1);

		if(size > 0)
		{
			buf[size] = '\0';

			printf("%s",buf);
			fflush(stdout);
		}
	}

//	int count = 5;
//	while(count-- > 0)
//	{
//		printf("Hello World \n");
//		fflush(stdout);
//	}

	close(fd);
	close(new_fd);

	return 0;
}


