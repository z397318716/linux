#include"communicate.h"

static int commMsgQueue(int flags){
  key_t key = ftok("/tmp", 0x6666);
  if(key < 0){
    perror("ftok");
    return -1;
  }

  int msg_id = msgget(key, flags);
  if(msg_id < 0){
    perror("msgget");
    return -1;
  }
  return msg_id;
}

int createMsgQueue(){
  return commMsgQueue(IPC_CREAT | IPC_EXCL | 0666);
}
int getMsgQueue(){
  return commMsgQueue(IPC_CREAT);
}
int destoryMsgQueue(int msg_id){
  if(msgctl(msg_id, IPC_RMID, NULL) < 0){
    perror("msgctl");
    return -1;
  }
  return 0;
}

int sendMsgQueue(int msg_id, int who, char *msg){
 struct mmsgbuf buf;
 buf.mtype = who;
 strcpy(buf.mtext, msg);

 if(msgsnd(msg_id, (void*)&buf, sizeof(buf.mtext), 0) < 0){
   perror("msgsnd");
   return -1;
 }
 return 0;
}
int recvMsgQueue(int msg_id, int recvType, char out[]){
  struct mmsgbuf buf;
  int size = sizeof(buf.mtext);
  if(msgrcv(msg_id, (void*)&buf, size, recvType, 0) < 0){
    perror("msgrcv");
    return -1;
  }
  strncpy(out, buf.mtext, size);
  out[size] = 0;
  return 0;
}
