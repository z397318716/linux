/*
 * 学习系统调用 IO 接口使用*/

#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
//#include<sys/types.h>
#include<sys/stat.h>


int main()
{
  int fd = -1;
  // 将当前进程文件的创建权限掩码设置为 0 -----仅在当前进程有效
  umask(0);
  // 注意, 一旦使用了 O_CREAT  必须指定权限, 
  fd = open("./test.txt", O_RDWR|O_CREAT, 0765);
  if(fd < 0)
  {
    perror("open file failed");
    return -1;
  }
  lseek(fd, 0, SEEK_END);
  char ptr[] = "好好学习, 天天向上!!!\n";
  int ret = write(fd, ptr, strlen(ptr));
  if(ret < 0)
  {
    perror("write file failed!");
    return -1;
  }
  printf("ret:%d\n", ret);
  lseek(fd, 0, SEEK_SET);
  char buf[1024] = {0};
  int red = read(fd, buf, sizeof(buf));
  if(red < 0)
  {
    perror("read file failed!");
    return -1;
  }
  printf("red: %d\n[%s]\n",red, buf);
  close(fd);
  return 0;
}
