/*
 * 演示进程退出方式
 * 1. main 中的 return   效果等同于 exit
 * 2. void _exit(int status); 系统调用
 *        退出一个进程,退出时刷新缓冲区+其它很多释放操作
 *    void exit(int status);  库函数
 *        粗暴退出一个进程,直接释放所有资源
 */
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>



int main()
{
 
  printf("------\n");
  sleep(3);
  _exit(0);



  int i = 0;
  for(i = 0; i < 255; i++)
  {
    /*
     * char *strerror(int errnum)
     * 返回上一条系统调用的错误描述
     * errno--每次系统调用完毕都会重置的一个全局变量
     * 用于存储这个系统调用的错误编号
     */
    printf("%s\n",strerror(i));
  }

  while(1)
  {
  }
  sleep(1);


  // 因为进程的退出码只用了一个字节来存储,
  // 所以进程的退出数据只能是一个小于256的数据
  return 0;
}
