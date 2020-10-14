#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

int main()
{
  while(1)
  {
    // 增加一个shell提示
    fflush(stdout); // 刷新标准缓冲区(没有换行,不会自动刷新缓冲区)
    printf("[minishell]$ ");
  
    char *ptr = buf;

    // 1. 等待标准输入
    char buf[1024] = {0};
    fgets(buf, 1023, stdin);
    // 将 buf 最后一位字符设置为 '\0'
    buf[strlen(buf) - 1] = '\0';
    printf("buf:[%s]\n", buf);
    // 2. 对输入数据进行解析
    char *argv[32] = { NULL };
    char *ptr = buf; 
    int argc = 0;
    while(*ptr != '\0')
    {
      if(*ptr != ' ')
      {
        argv[argc] = ptr;
        argc++;
        // 一直读取到下一个空格出现
        while(*ptr != ' ')
          ptr++;
        *ptr = '\0';
      }
      ptr++;
    }
    int i = 0;
    for(; i < argc; ++i)
    {
      printf("argv[%d] = [%s]\n", i, argv[i]);
    }
    // 3. 创建子进程
    // 4. 在子进程中程序替换
    pid_t pid = fork();
    if(pid == 0)
    {
      execvp(argv[0], argv);  // 程序替换成功后就去执行新的程序了, 不会继续向下走
      // 程序如果执行到这, 说明替换失败了
      perror("execvp error"); // 打印上一次系统调用接口报错原因
      exit(0);
      //    int ret = execvp(argv[0], argv);
      //    if(ret < 0)
      //    {
      //      // 打印上一次系统调用接口的报错原因
      //      perror("execvp error"); 
      //      return -1;
      //    }
      //    exit(0);
    }

    // 5. 进程等待
    wait(NULL);
  }



  return 0;
}
