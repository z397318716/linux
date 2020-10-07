/*
 * 在代码中访问环境变量*/
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

// 程序的运行参数可以通过 main 函数的参数传入
// argc 表示有多少个参数
// argv 中存储参数信息
int main(int argc, char* argv[], char* env[])
{
  int i = 0;
  for(i = 0; i < argc; ++i)
  {
    printf("argv[%d] = [%s]\n", i, argv[i]);
  }
  //for(i = 0; env[i] != NULL; ++i)
  //{
  //  printf("env[%d] = [%s]\n", i, env[i]);
  //}
  /*
  extern char** environ; // 声明一个变量, 这个变量实际上已经在其他地方定义了
  for(i = 0; environ[i] != NULL; ++i)
  {
    printf("environ[%d] = [%s]\n", i, environ[i]);
  }
  // char* getenv(const char* name) // 通过环境变量名称获取指定环境变量的数据
  */
  char* ptr = getenv("MYENV");
  if(ptr != NULL)
  {
    printf("%s\n", ptr);
  }
  else{
    printf("have no this varible\n");
  }


  return 0;
}
