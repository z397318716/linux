#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>




int main()
{
  
  pid_t id = fork();
  if(id < 0)
  {
    perror("fork");
    return 1;
  }
  else if(id == 0)
  {
    printf("child[%d] is begin Z...\n", getpid());
    sleep(5);
    exit(EXIT_SUCCESS);
  }
  else
  {
    printf("parent[%d] is sleeping...\n", getpid());
    sleep(30);
  }



  return 0;
}
