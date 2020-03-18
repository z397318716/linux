/*
 * 熟悉shell运行原理
 */

#include<stdio.h>
#include<errno.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<wait.h>

int main()
{
  while(1)
  {
    char cmd[1024] = {0};

    printf("minishell: ");
    fflush(stdout);
    // 正则表达式 scanf在读取缓冲区数据的时候,遇到 \n 终止读取
    // %[^\n]----  将 \n 之前的数据放到 cmd 中
    // %*c  -----  将剩下的一个字符全都从缓冲区取出来,丢弃
    // %*s  -----  将剩下的字符串全部都从缓冲区取出来,丢弃
    if(scanf("%[^\n]%*c", cmd) != 1)
    {
      getchar();
    }
    //scanf("%[^\n]%*s", cmd);
    printf("cmd:[%s]\n", cmd);
    char* ptr = cmd;
    char* argv[32] = {NULL};
    int argc = 0;
    while(*ptr != '\0')
    {
      if(!isspace(*ptr))
      {
        argv[argc++] = ptr;;
        while(!isspace(*ptr) && *ptr != '\0')
        {
          ptr++;
        }
        continue;
      }
      else
      {
        *ptr = '\0';
      }
      ptr++;
    }
    int i;
    for(i = 0; i < argc; i++)
    {
      printf("[%d]-[argv:[%s]]\n",i,argv[i]);
    }

    int pid = fork();
    if(pid < 0)
    {
      exit(-1);
    }
    else if(pid == 0)
    {
      execvp(argv[0], argv);
      exit(-1);
    }
    else
    {
      waitpid(pid, NULL, 0);
    }
  }
  return 0;
}
