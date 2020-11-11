#include<stdio.h>
#include<signal.h>
#include<stdlib.h>
#include<unistd.h>
int a = 1, b = 1;

int test(){

  a++;
  sleep(3);
  b++;
  return a + b;
}
void sigcb(int signum){
  printf("sum = %d\n",test());
}
int main(){
  signal(SIGINT, sigcb);
  printf("main---sum = %d\n", test());

  return 0;
}
