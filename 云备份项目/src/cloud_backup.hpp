#include <cstdio>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <zlib.h>
#include <pthread.h>
#include <iostream>
#include <boost/filesystem.hpp>
#include "httplib.h"

namespace _cloud_sys
{
  class FileUtil 
  {
    public:
      // 从文件中读取所有内容
      static bool Read(const std::string &name, std::string *body)
      {
        // 一定注意以二进制方式打开文件
        std::ifstream fs(name, std::ios::binary);  // 输入文件流
        if(fs.is_open() == false)
        {
          std::cout << "open file" << name << "failed\n";
          return false;
        }
        // boost::filesystem::file_size() 获取文件大小
        // 
        int64_t fsize = boost::filesystem::file_size(name); 
        // 给body申请
        body->resize(fsize);
        // &body[0]是取body中字符串的首地址
        // &body 是取这个string对象的首地址
        // ifstream.read(char* , int);
        // 因为body是一个指针,需要先解引用
        fs.read(&(*body)[0],fsize);
        if(fs.good() == false)
        {
          std::cout << "file" << name << "read data failed!" << std::endl;
          return false;
        }
        fs.close();
        return true;
      }
      // 向文件中写入数据
      static bool Write(const std::string &name, const std::string &body)
      {
        // 输出流---ofstream 默认打开文件的时候会清空原有内容
        // 当前的策略是覆盖写入(即便当前文件存在,也会用新文件覆盖)
        std::ofstream ofs(name, std::ios::binary);
        if(ofs.is_open() == false)
        {
          std::cout << "open file" << name << "failed\n";
          return false;
        }
        ofs.write(&body[0], body.size());
        if(ofs.good() == false)
        {
          std::cout << "file" << name << "write data failed!" << std::endl;
          return false;
        }
        ofs.close();
        return true;
      }
  };
  class CompressUtil
  {
    public:
      // 文件压缩(源文件名,目标文件名)
      static bool Compress(const std::string &src, const std::string &dst)
      {
        std::string body;
        FileUtil::Read(src,&body);

        // 打开压缩包
        gzFile gf = gzopen(dst.c_str(), "wb"); 
        if(gf == NULL)
        {
          std::cout << "open file" << dst << "failed!" << std::endl;
          return false;
        }
        // 由于gzwrite返回的是压缩的数据大小, 它有可能并未将数据压缩完(只压缩了一部分,返回了这部分的大小)
        // 所以这样的仅仅只执行一次是不合适的
        /*
        int ret = gzwrite(gf, &body[0], body.size());
        if(ret == 0)
        {
          std::cout << "file" << dst << "write compress data failed!" << std::endl;
          return false;
        }
        */

        // 所以考虑循环执行这个压缩过程
        int wlen = 0;
        // 这个循环压缩 就是避免body中的数据没有一次被压缩完
        while(wlen < body.size())
        {
          // 若一次没有将全部数据压缩, 则从 未压缩的数据开始压缩
          int ret = gzwrite(gf, &body[wlen], body.size() - wlen);
          if(ret == 0)
          {
            std::cout << "file" << dst << "write compress data failed" << std::endl;
            return false;
          }
          wlen += ret;
        }
        gzclose(gf);
        return true;
      }

        
      // 文件解压缩(压缩包名称, 源文件名称)
      static bool UnCompress(const std::string &src, const std::string &dst)
      {
        std::ofstream ofs(dst, std::ios::binary);
        if(ofs.is_open() == false)
        {
          std::cout << "open file " << dst << "failed!" << std::endl;
          ofs.close();
          return false;
        }
        gzFile gf = gzopen(src.c_str(), "rb");
        if(gf == NULL)
        {
          std::cout << "open file " << src << " failed!" << std::endl;
          ofs.close();
          return false;
        }
        char tmp[4096] = {0};
        int ret = 0;
        // gzread(句柄, 缓冲区, 缓冲区大小)
        // 返回实际读取到的解压后的数据大小
        while((ret = gzread(gf, tmp, 4096)) > 0)
        {
          ofs.write(tmp, ret);
        }
        ofs.close();
        gzclose(gf);

        return true;
      }
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
