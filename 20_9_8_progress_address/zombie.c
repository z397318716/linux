#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int main()
{
  pid_t pid = fork();
  if(pid < 0)
  {
    perror("fork error");
    return -1;

  }
  else if(pid == 0)
  {
    printf("child[%d] is begin Z...\n", getpid());
    sleep(5);//为了方便我们观察子进程退出之前的状态
    exit(0);

  }
  else
  {
    sleep(30);//为了使子进程先于父进程退出
    printf("parent[%d] is sleeping...\n", getpid());

  }
  while(1)
  {
    sleep(1);

  }
  return 0;

}
