/*
 * 通过封装的 tcpsocket类来实例化对象, 实现tcp 客户端通信*/
#include<iostream>
#include<stdlib.h>
#include"tcp_socket.hpp"

// 创建套接字
// 绑定地址信息---推荐操作系统自己绑定
// 向服务端发起请求
// 循环收发数据

int main(int argc, char *argv[]){

  if(argc != 3){
    printf("em:./tcp_cli 192.168.132.122 9000--服务器绑定的地址\n");
    return -1;
  }
  std::string ip = argv[1];
  //uint16_t port = std::stoi(argv[2]);
  uint16_t port = atoi(argv[2]);
  TcpSocket cli_sock;
  // 创建套接字
  CHECK_RET(cli_sock.Socket());
  // 绑定地址信息---推荐让操作系统去做---能够最大化的避免端口冲突
  // 向服务端发起请求
  CHECK_RET(cli_sock.Connect(ip, port));
  // 循环收发数据
  while(1){
    printf("client say: ");
    fflush(stdout);
    std::string buf;
    std::cin >> buf;

    // 因为客户端不存在多种套接字的问题, 因此一旦出现问题,直接退出
    // 进程退出就会释放资源, 关闭套接字
    CHECK_RET(cli_sock.Send(buf));

    buf.clear();
    CHECK_RET(cli_sock.Recv(&buf));
    printf("server say: %s\n", buf.c_str());
  }
  cli_sock.Close();


  return 0;
}
