/*
 * 学习mkfifo函数, 创建命名管道, 进行基本操作练习*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<errno.h>
#include<fcntl.h>

int main()
{
  // 涉及到权限操作, 不想计算, 直接设置权限掩码为0, 设置权限级别时就不需计算,
  umask(0);
  int ret = mkfifo("./test.fifo", 0664);
  if(ret < 0 && errno != EEXIST)
  {
    perror("mkfifo error!");
  }

  int fd = open("./test.fifo", O_RDONLY);
  if(fd < 0)
  {
    perror("open failed!");
  }
  printf("open fifo success\n");

  while(1){
    char buf[1024] = {0};
    int ret = read(fd, buf, 1023);
    if(ret < 0){
      perror("read error!");
      return -1;
    }
    else if(ret == 0){
      perror("all write closed!");
      return -1;
    }
    printf("read buf:[%s]\n", buf);
  }
  close(fd);

  return 0;
}
