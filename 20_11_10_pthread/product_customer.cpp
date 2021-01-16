#include<iostream>
#include<queue>
#include<pthread.h>

class BlockQueue{
  public:
    BlockQueue();
    //编码风格
    // 纯输入参数---const &
    // 输出型参数--- 指针
    // 输入输出型--- &
    bool Push(const int &data); // 入队数据
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
