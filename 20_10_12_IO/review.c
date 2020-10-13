/*
 * 回顾STL标准库中的 IO接口*/
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>


int main()
{
  FILE *fp = NULL;

  fp = fopen("./test.txt", "r+");
  if(fp == NULL)
  {
    perror("open file failed");
    return -1;
  }

  // SEEK_SET---从文件起始偏移
  // SEEK_CUR---从当前读写位置开始偏移
  // SEEK_END---从文件末尾偏移
  fseek(fp, 10, SEEK_END); 


  char buf[] = "理解文件流指针!!!\n";
  char *ptr = "理解文件流指针!!!\n";
  int ret = fwrite(buf, strlen(buf), 1, fp);
  // parameters: 数据,块大小,块个数,文件流指针
  if(ret == 0)
  {
    perror("fwrite error");
    return -1;
  }

  printf("write ret:%d\n", ret);

  fseek(fp, 0, SEEK_SET); 
  char tmp[1024] = {0};
  int red = fread(tmp, 1023, 1, fp);
  printf("red:%d-[%s]\n", red, tmp);




  fclose(fp);
  return 0;
}
