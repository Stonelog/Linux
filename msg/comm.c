#include "comm.h"

int create_msg(int flags)
{
	key_t _key = ftok(_PATH_NAME_,_PROJ_ID_);

	int msg_id = msgget(_key ,flags);
	if(msg_id < 0)
	{
		perror("msget");
	}
	return msg_id;
}

void init_msg()
{
	int flags = IPC_CREAT | IPC_EXCL;
	create_msg(flags);
}

int get_msg_id()
{
	int flags = IPC_CREAT;
	return  create_msg(flags);
}

void send_msg(int msg_id , char* msg)
{
	struct msgbuf _msgbuf;
	_msgbuf.mtype = 1 ;
	strcpy(_msgbuf.mtext,msg);
	
	ssize_t  _size = msgsnd(msg_id, &_msgbuf,sizeof(_msgbuf),0);
	if(_size < 0)
	{
		perror("msgsnd");
	}
}

void recv_msg(int msg_id,char *out)
{
	struct msgbuf _msgbuf;
	_msgbuf.mtype = 1 ;

	ssize_t _size =  msgrcv(msg_id, &_msgbuf, sizeof(_msgbuf), _msgbuf.mtype,0);
	if(_size < 0)
	{
		perror("msgrcv");
		return ;
	}

	strcpy(out,_msgbuf.mtext);
}

void destroy_msg(int msg_id)
{
	if(msgctl(msg_id,IPC_RMID,NULL) < 0)
	{
		perror("msgctl");
	}
}


