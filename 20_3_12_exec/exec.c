/*
 * 2020年3月12日13:32:20
 *  程序替换函数测试demo
 *  int execl(const char *path, const char *arg, ...);
 *  int execlp(const char *file, const char *arg, ...);
 *  int execle(const char *path, const char *arg, ...,char *const envp[]);
 *  int execv(const char *path, char *const argv[]);
 *  int execvp(const char *file, char *const argv[]);
 */
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>



int main()
{

  execl("/root/learn/20_3_12_exec/print.c","NULL");
  printf("------\n");


  return 0;
}
