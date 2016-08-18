#include <stdio.h>
#include "comm.h"

int main()
{
	int msg_id = get_msg_id();

	char buf[1024];

	while(1)
	{
		memset(buf,'\0',sizeof(buf));
		printf("client >: ");
		fflush(stdout);
		ssize_t _size = read(1,buf,sizeof(buf));

		if(_size <  0)
		{
			perror("read");
		}

		buf[_size-1]='\0';
		send_msg(msg_id,buf);

		memset(buf,'\0',sizeof(buf));
		recv_msg(msg_id,buf);
		printf("server >: ");
		fflush(stdout);
		printf("%s\n",buf);
		fflush(stdout);
	}
	
	return 0;
}


