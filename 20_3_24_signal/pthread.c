#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>

void* rout(void* arg)
{
  int i;
  for(;;)
  {
    printf("i am thread 1\n");
    sleep(1);
  }
}

int main()
{
  
  pthread_t tid;
  int ret;
  if((ret = pthread_create(&tid, NULL, rout, NULL)) != 0)
  {
    fprintf(stderr, "pthread_create: %s\n", strerror(ret));
    exit(EXIT_FAILURE);
  }
  int i;
  for(;;)
  {
    printf("i am main thread\n");
    sleep(1);
  }



  return 0;
}
