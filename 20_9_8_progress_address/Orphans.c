#include<stdio.h>
#include<unistd.h>
int main()
{
  pid_t pid = fork();
  if(pid < 0)
  {
    return -1;
  }
  else if(pid == 0)
  {
    sleep(10);//让子进程睡的时间长一点，让父进程先退出
    printf("this is child\n");
  }
  else
  {
    sleep(5);//这里睡几秒是为了执行后观察父进程退出之前的状态
    printf("this is parent\n");
  }
  return 0;
}
