/*
 * 通过匿名管道实现进程间通信
 * 体会 pipe 接口使用
 * */

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

int main()
{

  // 匿名管道只能用于具有亲缘关系的进程间通信, 子进程通过复制父进程的方式获取操作句柄
  // 创建管道的操作要在创建子进程之前, 否则 子进程复制父进程就复制不到这个管道了
  int pipefd[2] = {-1};
  int ret = pipe(pipefd);
  if(ret < 0)
  {
    perror("pipe error!");
    return -1;
  }
  pid_t pid = fork();
  if(pid < 0)
  {
    perror("fork error!");
  }
  else if(pid == 0)
  {
    // 子进程
    close(pipefd[1]);
    sleep(3);   // 关闭读端后, 休眠一段时间,不让继续读了
    char buf[1024];
    int rlen = read(pipefd[0], buf, 1023);
    printf("child:%d---%s", rlen, buf);
  }
  else{
    // 父进程
    close(pipefd[1]);   // 注意:要关闭所有的读端
    sleep(100);
    char *ptr = "我真的好菜啊!\n";
    int total_len = 0;
    while(1)
    {
      int wlen = write(pipefd[1], ptr, strlen(ptr));
      total_len += wlen;
      printf("total_len = %d\n", total_len);
    }
  }



  return 0;
}
