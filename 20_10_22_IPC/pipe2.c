/**
 * 通过匿名管道实现管道符的作用
 * 实现: ps -ef | grep ssh
 * ps -ef 运行结束后将进程信息输出到标准输出
 * grep -ssh 从标准输入读取数据进行过滤
 */
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<wait.h>

int main()
{
  int pipefd[2] = {-1};
  if(pipe(pipefd) < 0){
    perror("pipe error");
    return -1;
  }
  pid_t ls_pid = fork();
  if(ls_pid < 0){
    perror("ls_fork error");
    return -1;
  }
  else if(ls_pid == 0)
  {
    // ps子进程
    dup2(pipefd[1], 1); // 将标准输出重定向到管道写端
    execlp("ls", "ls", "-l", NULL);
    exit(0);
  }
  pid_t grep_pid = fork();
  if(grep_pid == 0){
    // grep 子进程
    // 关闭写端---ps进程一旦推出, 所有的写端被关闭, grep读完数据后返回0
    close(pipefd[1]);
    dup2(pipefd[0], 0); // 将标准输入重定向到管道读取端
    execlp("grep", "grep", "a", NULL);
    exit(0);
  }
  close(pipefd[0]);
  close(pipefd[1]);
  waitpid(ls_pid, NULL, 0);
  waitpid(grep_pid, NULL, 0);


  return 0;
}
