#include<iostream>
#include<pthread.h>
#include<unistd.h>
#include<mutex>
#include<semaphore.h>

typedef void (*_handler)(int data);
class MyTask{

  private:
    int _data; // 要处理的数据
    handler_t _handler; // 处理数据的方法

  public:
    // 用户自己传入要处理的数据和方法, 组织出一个任务节点
    SetTask(int data, handler_t handler);
    Run(){
      return _handler(_data);
    }
};

