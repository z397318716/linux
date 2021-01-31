/*
 * 封装一个 udp socket 类, 向外提供简单的接口实现套接字编程
     1. 创建套接字
     2. 为套接字绑定地址信息
     3. 发送数据
     4. 接收数据
     5. 关闭套接字
     */
#include<unistd.h>
#include<string>
#include<stdlib.h>
#include<stdio.h>
#include<iostream>
#include<netinet/in.h>
#include<netinet/udp.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#define CHECK_RET(q) if((q) == false){return -1;}
using namespace std;
class UdpSocket{
  public:
    UdpSocket()
    :_sockfd(-1){}
    // 1. 创建套接字
    bool Socket(){
      // socket(地址域, 套接字类型, 协议类型);
      _sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
      if(_sockfd < 0){
        perror("socket error");
        return false;
      }
      return true;
    }
    // 2. 为套接字绑定地址信息
    bool Bind(const std::string &ip, uint32_t port){
      // 1. 定义IPv4 地址结构--->2.绑定地址
      struct sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_port = htons(port);
      addr.sin_addr.s_addr = inet_addr(ip.c_str());
      // 2. 绑定地址
      // bind(描述符, 统一地址结构 sockaddr*, 地址信息长度)
      socklen_t len = sizeof(struct sockaddr_in);
      int ret = bind(_sockfd, (struct sockaddr*)&addr, len);
      if(ret < 0){
        perror("bind error");
        return false;
      }
      return true;
    }
    // 3. 发送数据
    bool Send(const std::string &data, const std::string &ip, uint16_t port){
      // sendto(描述符, 数据, 长度, 选项, 对端地址, 地址长度)
      // 1. 定义对端地址信息的ipv4 地址结构
      struct sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_port = htons(port);
      addr.sin_addr.s_addr = inet_addr(ip.c_str());
      // 2. 向对端地址发送数据
      socklen_t len = sizeof(struct sockaddr_in);
      int ret = sendto(_sockfd, data.c_str(), data.size(), 0, (struct sockaddr*)&addr, len);
      if(ret < 0){
        perror("sendto error");
        return false;
      }
      return true;
    }
    bool Recv(std::string *buf, std::string *ip = nullptr, uint16_t *port = nullptr){
      // recvfrom(描述符, 缓冲区, 数据长度, 选项, 对端地址, 地址长度)
      struct sockaddr_in addr;
      socklen_t len = sizeof(struct sockaddr_in);
      char tmp[4096] = {0};
      // 此处如果直接传入 buf, buf是个指针, 如果指向NULL, 则在该语句直接出错
      //int ret = recvfrom(_sockfd, buf, )
      int ret = recvfrom(_sockfd, tmp, 4096, 0, (struct sockaddr*)&addr, &len);
      if(ret < 0){
        perror("recvfrom error");
        return false;
      }
      // 给buf申请ret大小的空间, 从tmp中拷贝ret长度的数据进去
      buf->assign(tmp, ret);
      // 为了接口灵活, 用户若不想获取地址信息, 则不再转换获取
      // 只有当用户 想要获取地址的时候, 这时候传入缓冲区, 我们在将数据写入进去
      if(ip != nullptr){
      // 将网络字节序整数IP地址转换为字符串地址, 返回
        *ip = inet_ntoa(addr.sin_addr);
      }
      if(port != nullptr){
      // 网络字节序转换为主机字节序
        *port = ntohs(addr.sin_port);
      }
      return true;
    }
    // 5. 关闭套接字
    void Close(){
      close(_sockfd);
      _sockfd = -1;
      return ;
    }

    ~UdpSocket(){

    }
  private:
    int _sockfd;

};

/*
 * 客户端要给服务端发送数据, 那么就需要知道服务端的地址信息, 因此通过程序运行参数传入服务端的地址信息*/
int main(int argc, char *argv[]){
  if(argc != 3){
    printf("em: ./udp_cli 192.168.122.132 9000\n");
    return -1;
  }
  std::string ip_addr = argv[1]; // 服务端地址信息
  uint16_t port_addr = atoi(argv[2]);

  UdpSocket sock;
  CHECK_RET(sock.Socket()); // 创建套接字
  // CHECK_RET(sock.Bind()); // 绑定地址信息
  while(1){
    // 获取一个标准输入的数据, 进行发送
    std::cout << "client say: ";
    fflush(stdout);
    std::string buf;
    std::cin >> buf; // 获取标准输入数据
    sock.Send(buf, ip_addr, port_addr); // 向指定的主机进程发送buf数据
    buf.clear();
    sock.Recv(&buf); // 因为客户端已经知道服务端地址了, 就不用了在获取了
    std::cout << "server say: " << buf << std::endl;
  }
  sock.Close();

  return 0;
}
