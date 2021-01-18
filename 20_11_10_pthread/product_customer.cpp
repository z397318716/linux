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
    //
    bool Push(const int &data); // 入队数据// 入队数据
    bool Pop(int *data); // 出队数据
  private:
    std::queue<int> _queue; // STL中queue容器,是非线程安全的---STL设计之初就是为了高性能---功能多了耦合度就高
    int _capacity; // 队列中节点的最大数量---数据也不能无限制添加, 内存耗尽程序就崩溃了

    pthread_mutex_t _mutex;
    pthread_cond_t _productor_cond; //生产者队列
    pthread_cond_t _customer_cond; // 消费者队列
    //
};
int main(){


  return 0;
}
