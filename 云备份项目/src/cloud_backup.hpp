#include <cstdio>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <zlib.h>
#include <pthread.h>
#include <iostream>
#include "httplib.h"

namespace _cloud_sys
{
  class FileUtil 
  {
    public:
      // 从文件中读取所有内容
      static Read(const std::string &name, std::string &body)
      {
        // 一定注意以二进制方式打开文件
        std::fstream fs(name, std::ios::binary);  // 输入文件流
        if(fs.is_open() == false)
        {
          std::cout << "open file" << name << "failed\n";
          return false;
        }
        // boost::filesystem::file_size()
        //
        int64_t fsize = boost::filesystem::file_size(name); 
      }
      // 向文件中写入数据
      static Write(const std::string &name, const std::string &body); 
  };
  class CompressUtil
  {
    public:
      // 文件压缩(源文件名,目标文件名)
      static Compress(const std::string &src, const std::string *dst);  
      // 文件解压缩(压缩包名称, 源文件名称)
      static UnCompress(const std::string &src, const std::string *dst); 
  };
  class DataManger
  {
    public:
      DataManger()
      {

      }
      ~DataManger()
      {

      }
      // 判断文件是否存在
      bool Exists(const std::string &name);
      // 判断文件是否被压缩
      bool IsCompress(const std::string &name);
      // 获取未压缩文件
      bool NonCompressList(std::vector<std::string> *list);  
      // 插入/更新数据
      bool Insert(const std::string &src, const std::string &dst); 
      // 获取所有文件名称
      bool GetAllName(std::vector<std::string> *list); 
      // 数据改变后持久化存储
      bool Storage();  
      // 启动时初始化加载原有数据
      bool InitLoad();  
    private:
      // 持久化数据存储文件名称
      std::string _back_file; 
      // 数据管理器
      std::unordered_map<std::string, std::string> _file_list;  
      pthread_rwlock_t _rwlock;
  };
  class NonHotCompress
  {
    public:
      NonHotCompress()
      {

      }
      ~NonHotCompress()
      {

      }
      // 总体向外提供的功能接口, 开始压缩模块
      bool Start(); 
    private:
      // 判断一个文件是否是热点文件
      bool FileIsHot(const std::string &name);  
    private:
      // 压缩后的文件存储路径
      std::string _gz_dir;  
  };
  class Server
  {
    public:
      Server()
      {

      }
      ~Server()
      {

      }
      // 启动网络通信模块接口
      bool Start(); 
    private:
      // 文件上传处理回调函数
      static void FileUpLoad(const httplib::Request &req, httplib::Response &rsp);  
      // 文件列表获取请求函数
      static void List(const httplib::Request &req, httplib::Response &rsp);  
      // 文件下载处理回调函数
      static void FileDownload(const httplib::Request &req, httplib::Request &rsp); 
    private:
      // 文件上传备份路径
      std::string _file_dir;  
      httplib::Server _server;
  };

}
