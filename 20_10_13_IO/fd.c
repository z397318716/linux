#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>

int main()
{
  // 关闭0号描述符, 就是关闭了标准输入
  close(0);
  umask(0);
  int fd = open("./test.txt", O_RDWR|O_CREAT, 0664);
  if(fd < 0)
  {
    perror("open file failed!");
    return -1;
  }

  dup2(fd, 1);
  printf("fd = %d\n", fd);
  fflush(stdout); // 刷新标准输出缓冲区


  close(fd);
  return 0;
}
