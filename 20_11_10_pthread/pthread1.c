#include<stdio.h>
#include<stdlib.h>   
#include<unistd.h>   // sleep 接口头文件
#include<string.h>
#include<pthread.h>  // 线程库接口头文件

void *thr_start(void *arg){

  while(1){
    printf("thread ~~%s\n", (char*)arg);
    sleep(1);
  }
  return NULL;
}

int main(){
  
  // pthread_create(获取线程id, 线程属性, 线程入口函数, 参数)
  pthread_t tid;
  char ptr[] = "nihao";
  int ret = pthread_create(&tid, NULL, thr_start, (void*)ptr);
  if(ret != 0){
    printf("create thread failed!!! \n");
    return -1;
  }
  printf("create thread success!! \n");
  while(1){
    printf("我真的好菜啊11\n");
    sleep(1);
  }


  return 0;
}
