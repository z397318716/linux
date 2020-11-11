#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

void *thr_a(void *arg){
  printf("i am child thread\n");
  char *ptr = "normal thread exit!";
  sleep(5);
  pthread_exit(ptr);
  return NULL;
}

int main(){
  
  pthread_t tid;
  int ret = pthread_create(&tid, NULL, (void*)thr_a, NULL);
  if(ret != 0){
    printf("thread create failed!\n");
    return -1;
  }
  char *ptr;
  pthread_join(tid, (void**)&ptr);
  printf("%s\n", ptr);
  printf("main thread exit!\n");
  //pthread_exit(&tid);
 // pthread_cancel(tid);
  return 0;
}

