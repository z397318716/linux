/*
 * 使用封装的 TcpSocket 类实例化对象实现tcp服务端程序*/
#include<iostream>
#include<thread>
#include<stdlib.h>
#include<signal.h>
#include<wait.h>
#include"tcp_socket.hpp"

void sigcb(int signo){
  // 当子进程退出的时候就会向父进程发送 SIGCHLD 信号, 回调这个函数
  // waitpid 返回值 > 0 表示处理了一个退出的子进程
  // waitpid 返回值 < 0 表示没有退出的子进程了
  while(waitpid(-1, 0, WNOHANG) > 0);// 一次回调 循环将退出的所有子进程处理掉
}
int main(int argc, char *argv[]){

  if(argc != 3){
    printf("em:/tcp_srv 192.168.122.132 9000\n");
    return -1;
  }
  std::string ip = argv[1];
  //uint16_t port = std::stoi(argv[2]); // stoi 将字符串转换为数字
  uint16_t port = atoi(argv[2]); // stoi 将字符串转换为数字
  
  signal(SIGCHLD, sigcb);

  TcpSocket lst_sock;
  CHECK_RET(lst_sock.Socket());
  CHECK_RET(lst_sock.Bind(ip, port));
  CHECK_RET(lst_sock.Listen()); // 开始监听

  while(1){
    TcpSocket cli_sock;
    std::string cli_ip;
    uint16_t cli_port;
    // Accept 类成员函数, 使用的是私有成员 _sockfd就是lst_sock 对象的私有成员
    // cli_sock 取地址传入, 目的是为了获取 accept 接口返回的通信套接字描述符
    bool ret = lst_sock.Accept(&cli_sock, &cli_ip, &cli_port);
    if(ret == false){
      // 获取新连接失败, 重新获取
      continue;
    }
    printf("new connect:[%s:%d]\n", cli_ip.c_str(), cli_port);
    // --------------------------------------------------------------------------------------
    // 让子进程负责与客户端进行通信
    pid_t pid = fork();
    if(pid == 0){
      while(1){
        std::string buf;
        // 因为父子进程代码共享,数据独有, 所以子进程可以直接对 cli_sock 进行操作
        if(cli_sock.Recv(&buf) == false){
          cli_sock.Close();// 通信套接字接收数据出错, 关闭的是通信套接字
          exit(0);
        }
        printf("client:[%s:%d] say:%s\n", &cli_ip[0], cli_port, &buf[0]);
        std::cout << "server say: ";
        fflush(stdout);
        buf.clear();
        std::cin >> buf;

        if(cli_sock.Send(buf) == false){
          cli_sock.Close();
          exit(0);
        }
      }
      cli_sock.Close();
      exit(0);
    }
    // 父子进程数据独有, 都会具有 cli_sock, 但是父进程并不进行通信
    cli_sock.Close(); // 这个关闭对子进程没有影响, 因为数据各自有一份
   //wait(NULL); // 等待子进程退出, 避免出现僵尸进程(但是程序又有可能阻塞在这里---考虑信号)
  }
  lst_sock.Close();

  return 0;
}
