/*
 * 这个demo用于体会
 * 1. 如何创建一个进程, 以及创建的进程的各项细节
 * 2. 僵尸进程的实现()*/
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main()
{
  
  printf("----create child process start---%d\n", getpid());
  
  pid_t pid; // 定义这个变量只是开辟了一块空间
  pid = fork(); // 这个函数的返回值对于父子进程是不一样的
  if(pid == 0)
  {
    // 这是子进程进入的判断, 因为对于子进程fork的返回值是0
    printf("this is child %d--rpid: %d\n", getpid(), pid);
    sleep(5);
    // 这是观察僵尸进程的产生----子进程先于父进程退出,父进程忽略了子进程的退出原因
    // exit(0); // 退出当前调用进程---谁调用,谁退出----当前代码只有子进程可以运行到
  }
  else if(pid > 0)
  {
    // 这是父进程进入的判断, 对于父进程 fork的返回值是子进程的 pid > 0
    printf("this is parents %d--rpid: %d\n",getpid(), pid);
    exit(0);
  }
  printf("------create child process end---\n");


  return 0;
}
