#include<iostream>
#include<unistd.h>
int main(){

  pid_t pid = getpid();
  while(1){
    std::cout << "死循环-" << pid << std::endl;
    sleep(1);
  }
  return 0;
}
