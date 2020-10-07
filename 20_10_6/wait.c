#include<stdio.h>   // print
#include<unistd.h>  // sleep fork
#include<stdlib.h>  // exit
#include<sys/wait.h>




int main()
{
  pid_t pid = fork();
  if(pid == 0)
  {
    //child
    sleep(5);
    exit(0);  // 子进程5秒后退出, 父进程如果不进行任何处理就会变成僵尸进程
  }
  // wait(NULL); // 传 NULL 表示不考虑获取返回值
  int ret, status;
  while((ret == waitpid(pid, &status, WNOHANG)) == 0)
  {
    printf("现在子进程还没有退出\n");
    sleep(1);
  }
  //waitpid(-1, NULL, 0); //
  while(1)
  {
    printf("--------1----------\n");
    sleep(1);
  }



  return 0;
}
