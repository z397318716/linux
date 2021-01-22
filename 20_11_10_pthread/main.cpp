#include"pthread_pool.hpp"

void test_func(int data){
  int sec = (data % 3) + 1;
  printf("tid:%p--get data:%d, sleep:%d\n", pthread_self(), data, sec);
  sleep(sec);
}
void tmp_func(int data){
  int sec = (data % 3) + 1;
  printf("tid:%p--tmp_func\n", pthread_self());
  sleep(sec);

}

int main(){
  ThreadPool pool;
  for(int i = 0; i < 10; ++i){
    MyTask task;
    if(i % 2 == 0)
      task.SetTask(i, test_func);
    else
      task.SetTask(i, tmp_func);
    pool.TaskPush(task);
  }
  sleep(100);

  return 0;
}
