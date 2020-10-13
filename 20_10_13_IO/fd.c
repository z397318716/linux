#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>


int main()
{
  // 关闭0号描述符, 就是关闭了标准输入
  close(0);
  umask(0);
  int fd = open("./test.txt", O_RDWR|O_CREAT, 0664);
  



  return 0;
}
