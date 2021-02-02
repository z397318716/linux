/*
 * 封装实现一个 TCP socket 类, 向外提供简单接口;
 * 使外部通过实例化一个 tcp socket 对象就能完成tcp通信程序的建立*/

/*
 * 1. 创建套接字
 * 2. 绑定地址信息
 * 3. 服务端开始监听
 * 4. 客户端发起连接请求
 * 5. 服务端获取新建连接
 * 6. 发送数据
 * 7. 接收数据
 * 8. 关闭套接字*/
#include<iostream>
#include<unistd.h>
#include<string>
#include<stdlib.h>
#include<cstdio>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#define BACKLOG 10
#define CHECK_RET(q) if((q) == false){return -1;}
class TcpSocket{
  public:
    TcpSocket():_sockfd(-1){
      
    }
    ~TcpSocket(){}
    // 创建套接字
    bool Socket(){
      _sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
      if(_sockfd < 0){
        perror("socket error");
        return false;
      }
      return true;
    }
    void Addr(struct sockaddr_in *addr, const std::string &ip, uint16_t port){
      addr->sin_family = AF_INET;
      addr->sin_port = htons(port);
      inet_pton(AF_INET, ip.c_str(), &(addr->sin_addr.s_addr));
    }
    // 绑定地址信息
    bool Bind(const std::string &ip, const uint16_t port){
      // 1. 定义IPv4地址结构
      struct sockaddr_in addr;
      Addr(&addr, ip, port);
      socklen_t len = sizeof(struct sockaddr_in);
      int ret = bind(_sockfd, (struct sockaddr*)&addr, len);
      if(ret < 0){
        perror("bind error");
        return false;
      }
      return true;
    }
    // 服务端开始监听
    bool Listen(int backlog = BACKLOG){
      // listen(描述符, 同一时间的并发连接数)
      int ret = listen(_sockfd, backlog);
      if(ret < 0){
        perror("listen error");
        return false;
      }
      return true;
    }
    // 客户端发起连接
    bool Connect(const std::string &ip, const uint16_t port){
      // 1. 定义IPv4地址结构, 赋予服务端地址信息
      struct sockaddr_in addr;
      Addr(&addr, ip, port);
      // 2. 向服务端发起请求
      // 3. connect(客户端描述符, 服务端地址信息, 地址长度)
      socklen_t len = sizeof(struct sockaddr_in);
      int ret = connect(_sockfd, (struct sockaddr*)&addr, len);
      if(ret < 0){
        perror("connect error");
        return false;
      }
      return true;
    }
    // 服务端获取新建连接
    bool Accept(TcpSocket *sock, std::string *ip = nullptr, uint16_t *port = nullptr){
      // accept(监听套接字, 对端地址信息, 地址信息长度) 返回新的描述符
      struct sockaddr_in addr;
      socklen_t len = sizeof(struct sockaddr_in);
      // 获取新的套接字及其对端信息, 将其放入addr中
      int clisockfd = accept(_sockfd, (struct sockaddr*)&addr, &len);
      if(clisockfd < 0){
        perror("accept error");
        return false;
      }
      // 用户传入了一个tcpsocket 空对象, 为该对象的描述符赋值, 以便后续通过该对象与客户端进行通信
      sock->_sockfd = clisockfd;
      if(ip != nullptr){
        *ip = inet_ntoa(addr.sin_addr); // 网络字节序ip -> 字符串IP
      }
      if(port != nullptr){
        *port = ntohs(addr.sin_port);
      }
    }
    // 发送数据
    bool Send(const std::string &data){
      // send(描述符, 数据, 数据长度, 选项参数)
      int ret = send(_sockfd, data.c_str(), data.size(), 0);
      if(ret < 0){
        perror("send error");
        return false;
      }
      return true;
    }
    // 接收数据
    bool Recv(std::string *buf){
      // recv(描述符, 缓冲区, 数据长度, 选项);
      char tmp[4096] = {0};
      int ret = recv(_sockfd, tmp, 4096, 0);
      if(ret < 0){
        perror("recv error");
        return false;
      }else if(ret == 0){
        std::cout << "connection break!" << std::endl;
        return false;
      }
      buf->assign(tmp, ret);
      return true;
    }
    // 关闭套接字
    void Close(){
      close(_sockfd);
      _sockfd = -1;
    }
  private:
    int _sockfd;
};
