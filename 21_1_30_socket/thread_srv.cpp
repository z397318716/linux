/*
 * 使用封装的 TcpSocket 类实例化对象实现tcp服务端程序*/
#include<iostream>
#include<stdlib.h>
#include"tcp_socket.hpp"

void *thr_start(void *arg){
  long fd = (long)arg;
  TcpSocket cli_sock;
  cli_sock.SetFd(fd);
  while(1){
    std::string buf;
    if(cli_sock.Recv(&buf) == false){
      cli_sock.Close();// 通信套接字接收数据出错, 关闭的是通信套接字
      pthread_exit(NULL);
    }
    printf("client say:%s\n", &buf[0]);
    std::cout << "server say: ";
    fflush(stdout);
    buf.clear();
    std::cin >> buf;

    if(cli_sock.Send(buf) == false){
      cli_sock.Close();
      pthread_exit(NULL);
    }

  }
}
  int main(int argc, char *argv[]){

    if(argc != 3){
      printf("em:/tcp_srv 192.168.122.132 9000\n");
      return -1;
    }
    std::string ip = argv[1];
    //uint16_t port = std::stoi(argv[2]); // stoi 将字符串转换为数字
    uint16_t port = atoi(argv[2]); // stoi 将字符串转换为数字

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
      //------------------------------------------------------------------------------------
      // 新连接到来, 使用多线程方式进行处理
      pthread_t tid;
      pthread_create(&tid, NULL, thr_start,(void*)cli_sock.GetFd());
      pthread_detach(tid); // 不关心线程的返回值,线程分离,线程退出后,自动释放资源 
      // 主线程不能关闭 cli_sock 套接字, 因为多线程是共用描述符的
    }
    lst_sock.Close();

    return 0;
}
