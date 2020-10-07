#include<stdio.h>
#include<unistd.h>
int gval = 1;
int main()
{
  pid_t pid = 0;
  pid = fork();
  if(pid == 0)
  {
    //子进程
    gval = 100;
    printf("child gval: %d---地址: %p\n", gval, &gval);
  }
  else if(pid > 0)
  {
    // 父进程
    sleep(3); // 这个休眠3秒钟表示,子进程修改了 gval 的值之后在打印
    printf("parent gval: %d---地址: %p\n", gval, &gval);
  }
  return 0;
}
