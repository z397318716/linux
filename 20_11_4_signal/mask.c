#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>

void sigcb(int signum){
  printf("recv a signal: %d\n", signum);
}

int main(){
  
  signal(SIGINT, sigcb);
  signal(SIGRTMIN+4, sigcb);

  sigset_t set;
  sigemptyset(&set); // 清空集合, 防止未知数据造成影响
  sigfillset(&set); // 向集合中添加所有信号
  sigprocmask(SIG_BLOCK, &set, NULL); // 阻塞set集合中的所有信号

  printf("press enter continue\n");
  getchar(); // 等待一个回车, 如果不按回车就一直卡在这里

  sigprocmask(SIG_UNBLOCK, &set, NULL); // 解除阻塞

  while(1){
    sleep(1);
    printf("睡一秒\n");
  }

  return 0;
}
