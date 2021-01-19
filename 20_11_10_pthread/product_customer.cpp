#include<iostream>
#include<queue>
#include<pthread.h>

#define QUEUE_MAX 5
// 线程安全的阻塞队列---没有数据则消费者阻塞 / 数据满了则生产者阻塞
class BlockQueue{
  public:
    BlockQueue(int maxq = QUEUE_MAX)
      :_capacity(maxq){
        pthread_mutex_init(&_mutex, NULL);
        pthread_cond_init(&_productor_cond, NULL);
        pthread_cond_init(&_customer_cond, NULL);
      }
    ~BlockQueue(){
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_productor_cond);
        pthread_cond_destroy(&_customer_cond);
    }
    //编码风格
    // 纯输入参数---const &
    // 输出型参数--- 指针
    // 输入输出型--- &
    
    // 入队数据
    bool Push(const int &data){
      // 生产者才会入队数据， 如果队列中数据满了则需要阻塞
      pthread_mutex_lock(&_mutex);
      // _queue.size() 获取队列节点个数
      while(_queue.size() == _capacity){
        // 队列满了, 进行阻塞操作
        pthread_cond_wait(&_productor_cond, &_mutex);
      }
      _queue.push(data);// 入队操作
      pthread_mutex_unlock(&_mutex);//解锁
      pthread_cond_signal(&_customer_cond); // 唤醒消费者
      
      return true;
    }
    // 出队数据
    bool Pop(int *data){
      // 出队都是消费者, 有数据才能出队, 没有数据则阻塞
      while(_queue.empty()){
        pthread_cond_wait(&_customer_cond, &_mutex);
      }
      // 获取队首节点数据
      *data = _queue.front();
      _queue.pop(); // 出队
      pthread_mutex_unlock(&_mutex);
      // 唤醒生产者
      pthread_cond_signal(&_productor_cond);
      return true;
    }
  private:
    std::queue<int> _queue; // STL中queue容器,是非线程安全的---STL设计之初就是为了高性能---功能多了耦合度就高
    int _capacity; // 队列中节点的最大数量---数据也不能无限制添加, 内存耗尽程序就崩溃了

    pthread_mutex_t _mutex;
    pthread_cond_t _productor_cond; //生产者队列
    pthread_cond_t _customer_cond; // 消费者队列
    //
};
int main(){

  int ret, i;
  pthread_t ptid[4], ctid[4];
  BlockQueue queue;

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
    pthread_join(&ptid[i], NULL);
    pthread_join(&ctid[i], NULL);
  }

  return 0;
}
