#pragma once

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>


#define _PATH_NAME_ "/tmp/"
#define _PROJ_ID_  0x555
#define _SIZE_ 1024

struct msgbuf 
{
   long mtype;       /* message type, must be > 0 */
   char mtext[_SIZE_];    /* message data */
};

static int create_msg(int flags);

void init_msg();

int get_msg_id();

void send_msg(int msg_id , char* msg);

void recv_msg(int msg_id,char *out);

void destroy_msg(int msg_id);


