/*
 * 使用封装的 TcpSocket 类实例化对象实现tcp服务端程序*/
#include<iostream>
#include"tcp_socket.hpp"


int main(int argc, char *argv[]){

  if(argc != 3){
    printf("em:/tcp_srv 192.168.122.132 9000\n");
    return -1;
  }
  std::string ip = argv[1];
  uint16_t port = std::stoi(argv[2]); // stoi 将字符串转换为数字

  TcpSocket lst_sock;
  CHECK_RET(lst_sock.Socket());
  CHECK_RET(lst_sock.Bind(ip, port));
  CHECK_RET(lst_sock.Listen());

  while(1){
    TcpSocket cli_sock;
    std::string cli_ip;
    uint16_t cli_port;
    // Accept 类成员函数, 使用的是私有成员 _sockfd就是lst_sock 对象的私有成员
    // cli_sock 取地址传入, 目的是为了获取 accept 接口返回的通信套接字描述符
    bool ret = lst_sock.Accept(&cli_sock, &cli_ip, &cli_port);
    std::string buf;
    if(cli_sock.Recv(&buf) == false){
      cli_sock.Close();// 通信套接字接收数据出错, 关闭的是通信套接字
      continue;
    }
    printf("client:[%s:%d] say:%s\n", &cli_ip[0], cli_port, &buf[0]);
    std::cout << "server say: ";
    fflush(stdout);
    buf.clear();
    std::cin >> buf;

    if(cli_sock.Send(buf) == false){
      cli_sock.Close();
      continue;
    }
  }
  lst_sock.Close();

  return 0;
}
