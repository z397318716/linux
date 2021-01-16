#ifndef _COMMUNICATE_H_
#define _COMMUNICATE_H_
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<sys/msg.h>

#define SERVER_TYPE 1
#define CLIENT_TYPE 2
struct mmsgbuf{
  long mtype;
  char mtext[1024];
}msgbuf;

int createMsgQueue();
int getMsgQueue();
int destoryMsgQueue(int msg_id);
int sendMsgQueue(int msg_id, int who, char *msg);
int recvMsgQueue(int msg_id, int recvType, char out[]);


#endif
