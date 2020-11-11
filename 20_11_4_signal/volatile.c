#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
volatile long long a = 1;
void sigcb(int signum){
  a = 0;
  printf("a = %d---signum = %d\n", a, signum);
}
int main(){

  signal(SIGINT, sigcb);
  while(a){

  }
  printf("exit----%d\n", a);

  return 0;
}
