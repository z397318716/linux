#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>



int main()
{
  char buf[] = " ls  -l  -a";
  char *ptr = buf;
  char *n_argv[32] = {NULL};
  int n_argc = 0;
  while(*ptr != '\0')
  {
    // 当前字符不为空格
    if(*ptr != ' ')
    {
      n_argv[n_argc] = ptr;
      n_argc++;
      while(*ptr != ' ')
      {
        ptr++;
      }
      *ptr = '\0';
    }
    ptr++;
  }
  
  int i;
  for(i = 0; i < n_argc; ++i)
  {
    printf("n_argv[%d] = [%s]\n", i, n_argv[i]);
  }



  return 0;
}

