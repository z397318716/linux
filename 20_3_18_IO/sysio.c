/*
 * 系统调用 IO 接口demo
 * open   close   read   write    lseek
 */ 


#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<errno.h>


int main()
{
  /*
   *    int open(const char* pathname, int flags, mode_t mode);
   *    pathname  文件路径名
   *    flags     打开标志位
   *    mode      权限
   *        O_RDONLY    只读
   *        O_WRONLY    只写
   *        O_RDWR      读写
   *        O_CREAT     文件存在则打开, 不存在则创建
   *        O_EXCL      与 O_CREAT 同用, 文件存在则报错返回
   *        O_TRUNC     打开文件同时, 清空原有内容
   *        O_APPEND    追加
   *   返回值: 文件描述符(非负整数)
   */
  char* file = "./test.tmp";
  int fd = open(file, O_RDWR | O_CREAT,0640);
  if(fd < 0)
  {
    perror("open error");
    return -1;
  }
  close(fd);



  return 0;
}
