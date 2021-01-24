#include<iostream>
#include<unistd.h>
#include<thread>
#include<mutex>
/*
   class single_instance{
   public:
   single_instance(){

   }
   ~single_instance(){

   }
   int *get_instance(){
   return &_data;
   }
   private:
   static int _data;

   };
   int single_instance::_data = 10; // 静态成员变量需要在类外初始化
   */
std::mutex g_mutex;
class single_instance{
  public:
    single_instance(){

    }
    ~single_instance(){

    }
    volatile int *get_instance(){
      // 如果直接加锁, 就意味着,不管data资源是否被加载, 每次都要先加锁在判断, 然后解锁, 
      // 若资源已经加载过了, 加/解锁烂费资源, 可能会引起锁冲突, 
      // 所以可以先进行判断, 避免每次加锁解锁
      // 4.二次判断, 避免每次都加锁解锁
      if(_data == nullptr){
        g_mutex.lock();// 3. 实现线程安全
        if(_data == nullptr){
          _data = new int;
          *_data = 10;
        }
        g_mutex.unlock();
      }
      return _data;
    }
  private:
    // static int *_data; // 和饿汉模式不同的是, 这里的静态变量是指针, 初始化时只是初始化指针的指向, 并没有初始化数据data
    volatile static int *_data;//1. volatile 防止编译器过度优化 2. static 保证所有对象使用同一份资源
};
volatile int *single_instance::_data = nullptr;
int main(){

  single_instance s1;
  single_instance s2;

  printf("%d--%p\n",*(s1.get_instance()), s1.get_instance());
  printf("%d--%p\n",*(s2.get_instance()), s2.get_instance());


  return 0;
}
