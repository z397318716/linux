#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>



int main(int argc, char* argv[])
{

  printf("123\n");
  // 对当前调用进程进程程序替换, 运行 ls 程序
  execl("/usr/bin/ls", "ls", "-l", NULL);

  return 0;
}
