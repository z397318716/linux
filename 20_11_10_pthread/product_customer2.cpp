/*
 * 使用信号量实现生产者消费者---主要实现线程安全的阻塞队列
 * 队列使用数组实现(环形队列)
 * 使用了sem_init 函数  编译时链接线程库   -lpthread */
#include<iostream>
#include<vector>
#include<mutex>
#include<pthread.h>
#include<semaphore.h>
#define QUEUE_MAX 5

class RingQueue{
  public:
    RingQueue(int maxq = QUEUE_MAX)
      :_queue(maxq), _capacity(maxq),
      _step_read(0), _step_write(0){
        // sem_init(信号量, 进程/线程标志, 信号量初值)
        // int sem_init(sem_t *sem, int pshared, unsigned int value);
        sem_init(&_lock, 0, 1); // 用于实现互斥锁, 初始值为1, 表示有一个可以访问
        sem_init(&_sem_data, 0, 0); // 数据空间的计数初始为 0
        sem_init(&_sem_idle, 0, maxq); // 空闲空间计数初始节点个数
      }
    ~RingQueue(){
      sem_destroy(&_lock);
      sem_destroy(&_sem_data);
      sem_destroy(&_sem_idle);
    }
    bool Push(int data){
      // 1. 判断是否能够访问资源, 不能访问则阻塞--空闲空间计数的判断, 空闲空间计数-1
      sem_wait(&_sem_idle);
      // 2. 能访问则加锁, 保护访问过程
      sem_wait(&_lock); // lock 计数是否大于0, 当前若可以访问(>0)则计数-1, 别人就不能访问了
      // 3. 资源的访问
      _queue[_step_write] = data;
      _step_write = (_step_write + 1) % _capacity; // 走到最后, 从头开始
      // 4. 解锁
      sem_post(&_lock); // lock 计数+1, 唤醒其它因为加锁阻塞的线程
      // 5. 入队数据之后, 数据空间计数+1, 唤醒消费者
      sem_post(&_sem_data);
      return true;
    }
    bool Pop(int *data){
      sem_wait(&_sem_data); // 判断有没有数据
      sem_wait(&_lock); // 有数据则加锁保护访问数据的过程
      *data = _queue[_step_read]; // 获取数据
      _step_read = (_step_read + 1) % _capacity;
      sem_post(&_lock); // 解锁操作
      sem_post(&_sem_idle); // 取出数据, 则空闲空间计数+1, 唤醒生产者
      return true;
    }


  private:
    std::vector<int> _queue;// 数组实现 环形队列
    int _capacity;// 队列的容量
    int _step_read;// 获取数据---读指针的位置下标
    int _step_write;// 写入数据---写指针的位置下标
    /*
     * 互斥的实现一个信号量就够了, 这里定义了三个信号量
     * 一个用于实现互斥
     * 一个用于对空闲空间进行计数--- 实现入队操作的合理性判断
     * 一个用于对数据空间进行计数--- 实现出队操作的和理性判断*/
    sem_t _lock; // 这个信号量用于实现互斥
    sem_t _sem_idle; // 这个信号量用于对空闲空间进行计数---对于生产者来说有空闲空间的时候才能写数据--空闲空间>0 才能写数据
    sem_t _sem_data; // 这个信号量用于对具有数据的空间进行计数---对于消费者来说:有数据的空间计数 > 0 的时候才能取出数据
};
void *thr_productor(void *arg){
  RingQueue *queue = (RingQueue*)arg;
  int i = 0;
  while(1){
    // 生产者不断生产数据
    queue->Push(i);
    i++;
    printf("productor push data:%d\n", i);
  }
  return NULL;
}
void *thr_customer(void *arg){
  RingQueue *queue = (RingQueue*)arg;
  while(1){
    // 消费者不断的获取数据进行处理
    int data;
    queue->Pop(&data);
    printf("customer pop data:%d\n", data);
  }
  return NULL;
}

int main(){

  int ret, i;
  pthread_t ptid[4], ctid[4];
  RingQueue queue;

  for(i = 0; i < 4; i++){
    ret = pthread_create(&ptid[i], NULL, thr_productor, (void*)&queue);
    if(ret != 0){
      printf("create productor thread error\n");
      return -1;

    }
    ret = pthread_create(&ctid[i], NULL, thr_customer, (void*)&queue);
    if(ret != 0){
      printf("create productor thread error\n");
      return -1;

    }


  }
  for(i = 0; i < 4; ++i){
    pthread_join(ptid[i], NULL);
    pthread_join(ctid[i], NULL);

  }
  return 0;
}
