/*
 * 实现一个线程池*/
#include<iostream>
#include<queue>
#include<pthread.h>
#include<unistd.h>
#include<mutex>
#include<semaphore.h>
#define MAX_THREAD 10

typedef void (*handler_t)(int);
class MyTask{
  public:
    MyTask(){}

    // 用户自己传入要处理的数据和方法, 组织出一个任务节点
    void SetTask(int data, handler_t handler){
      _data = data;
      _handler = handler;
    }
    void Run(){
      return _handler(_data);
    }
  private:
    int _data; // 要处理的数据
    handler_t _handler; // 处理数据的方法

};

class ThreadPool{
  public:
    ThreadPool(int max_thr = MAX_THREAD)
      :_thr_max(max_thr)
    {
      pthread_mutex_init(&_mutex, NULL);
      pthread_cond_init(&_cond, NULL);

      for(int i = 0; i < _thr_max; ++i){
        pthread_t tid;
        int ret = pthread_create(&tid, NULL, thr_start, this);
        if(ret != 0){
          perror("thread create error!");
          exit(-1);
        }
      }
    }
    ~ThreadPool(){
      pthread_mutex_destroy(&_mutex);
      pthread_cond_destroy(&_cond);
    }
    bool TaskPush(MyTask &task){
      pthread_mutex_lock(&_mutex);
      _queue.push(task);
      pthread_mutex_unlock(&_mutex);
      //pthread_cond_signal(&_cond);// 唤醒消费者线程
      pthread_cond_broadcast(&_cond); // 唤醒所有消费者线程, 谁抢到谁处理
      return true;
    }
    // 类的成员函数, 有一个隐藏的默认参数, 是this指针, 这样的话, 就存在两个参数了 与线程的入口函数要求不匹配了
    // 所以要把 这个函数设计成 静态成员函数
    // 成员变量是每一个实例化对象都有一个独有的
    // 但是没有this指针, 函数不知道操作的到底是哪个实例化对象的成员变量
    // 考虑 C++ 在类外对私有变量的访问可以通过调用公有函数的形式来
    // 不推荐下面的这种写法
    static void *thr_start(void *arg){
      ThreadPool *p = (ThreadPool *) arg;
      // 不断的从任务队列中取出任务, 执行任务的Run接口
      pthread_mutex_lock(&p->_mutex);
      // 如果任务队列为 空, 则阻塞等待
      while(p->_queue.empty())
        pthread_cond_wait(&p->_cond, &p->_mutex);

      MyTask task;
      task = p->_queue.front();
      p->_queue.pop();
      pthread_mutex_unlock(&p->_mutex);
      // 任务的处理要放在解锁外, 当前的锁保护的是队列的操作
      task.Run();
      return nullptr;
    }


  private:
    // 定义线程池中线程的最大数量---初始化时创建相应数量的线程即可
    int _thr_max;
    std::queue<MyTask> _queue;
    // 实现 _queue 操作的安全性
    pthread_mutex_t _mutex;
    // 实现线程池中消费者线程的同步
    pthread_cond_t _cond;
};
