/*
 * 线程安全---同步--条件变量
 * 以顾客吃饭和厨师做饭的流程(一个碗作为条件变量)
 * 演示 条件变量 的基本流程与使用注意事项*/
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>

// 标签(碗) 0 表示碗中没有饭
int bowl = 0;
// 实现线程间对 bowl 变量访问的同步操作
pthread_cond_t cook_cond;
pthread_cond_t customer_cond;
// 保护bowl变量的访问操作
pthread_mutex_t mutex;

void *thr_cook(void *arg){
  while(1){
    pthread_mutex_lock(&mutex);
    while(bowl != 0){
      // 表示碗里有饭, 不满足做饭的条件
      // 让厨师线程 等待, 等待之前先解锁, 被唤醒之后在加锁
      // pthread_cond_wait 接口中就完成了解锁,休眠,被唤醒后加锁--并且解锁和休眠是一步完成, 保证原子操作
      pthread_cond_wait(&cook_cond, &mutex);
    }

    // 表示碗里没有饭, 则厨师做一碗饭, 将bowl 置1
    bowl = 1;
    printf("厨师:做了一碗饭\n");
    // 解锁
    pthread_mutex_unlock(&mutex);
    // 唤醒顾客吃饭
    pthread_cond_signal(&customer_cond);

  }
  return NULL;
}
void *thr_customer(void *arg){
  while(1){
    // 加锁
    pthread_mutex_lock(&mutex);
    while(bowl != 1){
      // 没有饭, 不满足吃饭条件, 等待
      // 等待前先解锁, 被唤醒后加锁
      pthread_cond_wait(&customer_cond, &mutex);
    }
    bowl = 0; // 执行到此处表示有饭 bowl == 1 , 吃完饭,将 bowl 修改为0
    printf("顾客:吃了一碗饭\n");
    // 唤醒厨师做饭
    pthread_cond_signal(&cook_cond);
    // 解锁
    pthread_mutex_unlock(&mutex);
  }
  return NULL;
}
int main(){
  pthread_t cook_tid[4], customer_tid[4];
  int ret, i;


  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&customer_cond, NULL);
  pthread_cond_init(&cook_cond, NULL);
  for(i = 0; i < 4; ++i){
    ret = pthread_create(&cook_tid[i], NULL, thr_cook, NULL);
    if(ret != 0){
      perror("cook pthread create failed!");
      return -1;
    }
    ret = pthread_create(&customer_tid[4], NULL, thr_customer, NULL);
    if(ret != 0){
      perror("customer pthread create failed!");
      return -1;
    }
  }
  pthread_join(cook_tid[0], NULL);
  pthread_join(customer_tid[0], NULL);

  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&customer_cond);
  pthread_cond_destroy(&cook_cond);

  return 0;
}
