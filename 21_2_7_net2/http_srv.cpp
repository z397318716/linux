/*
 * 使用封装的 TcpSocket 类实例化对象实现http服务端程序*/
#include<iostream>
#include<stdlib.h>
#include"tcp_socket.hpp"
#include<sstream>


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

    std::string http_req;
    cli_sock.Recv(&http_req);
    printf("req:[%s]\n", http_req.c_str());

    // 响应---首行(版本.状态码/描述)--头部(Content-Length) - 空行-正文
    std::string body = "<html><body><h1>Hello World</h1></body></html>";
    std::stringstream header;
    std::string blank = "\r\n";
    header << "Content-Length: " << body.size() << "\r\n";
    header << "Content-Type: text/html\r\n";
    header << "Location: http://www.baidu.com/\r\n";
    //std::string first_line = "HTTP/1.1 200 OK\r\n";// 体会状态码 200
    //std::string first_line = "HTTP/1.1 302 Found\r\n";// 体会状态码 302
    std::string first_line = "HTTP/1.1 404 Page NotFound\r\n";// 体会状态码 404
    
    cli_sock.Send(first_line); // 首行
    cli_sock.Send(header.str()); // 头部
    cli_sock.Send(blank); // 空行
    cli_sock.Send(body); // 正文
    cli_sock.Close();
  }

  lst_sock.Close();
  return 0;
}

