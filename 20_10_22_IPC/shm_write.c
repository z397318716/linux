/*
 * 学习共享内存的操作流程并体会接口的基本使用*/

// 1. 创建共享内存
// 2. 建立映射关系
// 3. 进行内存操作
// 4. 解除映射关系
// 5. 删除共享内存

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#define IPC_KEY 0x12345678

int main()
{
  // 1. 创建共享内存
  /* int shmget(key_t key, int size, int shmflag) key--标识符 size--共享内存大小  shmglag--标志位|权限*/
  int shm_id = shmget(IPC_KEY, 32, IPC_CREAT|0664);
  if(shm_id < 0)
  {
    perror("shmget error");
    return -1;
  }
  // 2. 建立映射关系
  /* void *shmat(int shmid, void *shmaddr, int shmflg)
   * shmid---操作句柄
   * shmaddr---映射首地址
   * shmflag---操作权限*/
  void *shm_start = shmat(shm_id, NULL, 0);
  if(shm_start == (void*)-1)
  {
    perror("shmat error");
    return -1;
  }
  // 3. 进行内存操作
  int i = 0;
  while(1)
  {
    sleep(1);
    sprintf(shm_start, "%s+%d", "我好菜啊", i++);
  }
  // 4. 解除映射关系
  // shmdt(映射首地址 )
  shmdt(shm_start);
  // 5. 删除共享内存
  // shmctl(操作句柄, 要进行的操作--IPC_RMID, 共享内存信息结构)
  shmctl(shm_id, IPC_RMID, NULL);

  return 0;
}
