/*
 * 学习修改信号的处理方式
 * sighandler_t signal(int signum, sighandler_t handler) 接口使用*/
#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>

void sigcb(int signum)
{
  printf("recv a signal num : %d\n", signum);
}
int main()
{
  // 1.将 SIGINT 处理方式修改为 SIG_IGN 忽略处理方式  
  //signal(SIGINT, SIG_IGN);
  // 2.将 SIGINT 处理方式修改为 自定义 处理方式  
  // 只是修改了内核中信号的回调函数指针
  signal(SIGINT, sigcb);
  while(1){
    printf("我好冷啊!!!\n");
    sleep(1);
  }


  return 0;
}
