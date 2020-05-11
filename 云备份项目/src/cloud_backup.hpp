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
#include <boost/algorithm/string.hpp>

namespace _cloud_sys
{
#define NONHOT_TIME 10   // 基准时间 (如果当前时间减去最后一次操作的时间大于该基准值,则认为该文件为非热点文件)
#define INTERVAL_TIME 30 // 非热点文件的检测间隔(30秒一次)
#define BACKUP_DIR "./backup/"  // 文件的备份路径 
#define GZFILE_DIR "./gzfile/"  // 压缩包存放路径
#define DATA_FILE "./list.backup" // 数据管理模块的数据备份文件名称
  DataManager data_manage(DATA_FILE);
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
  // 数据管理模块
  // 这个模块可以使用数据库来代替
  class DataManger
  {
    public:
      DataManger(const std::string &path):
        _back_file(path)
      {
        pthread_rwlock_init(&_rwlock, NULL);

      }
      ~DataManger()
      {
        pthread_rwlock_destroy(&_rwlock);

      }
      // 判断文件是否存在
      bool Exists(const std::string &name)
      {
        // 是否能够从 _file_list 中找到这个文件信息
        pthread_rwlock_rdlock(&_rwlock);
        auto it = _file_list.find(name);
        if(it == _file_list.end())
        {
          pthread_rwlock_unlock(&_rwlock);
          return false;
        }
        pthread_rwlock_unlock(&_rwlock);
        return true;

      }
      // 判断文件是否被压缩
      bool IsCompress(const std::string &name)
      {
        // 管理的数据: 源文件名称-压缩包名称
        // 文件上传后: 源文件名称和压缩包名称一致
        // 文件压缩后: 将压缩包名称更新为具体的包名
        pthread_rwlock_rdlock(&_rwlock);
        auto it = _file_list.find(name);
        if(it == _file_list.end())
        {
          pthread_rwlock_unlock(&_rwlock);
          return false;
        }
        // 如果两个名称一致, 则表示未压缩
        if(it->first == it->second)
        {
          pthread_rwlock_unlock(&_rwlock);
          return false;
        }
        pthread_rwlock_unlock(&_rwlock);
        return true;

      }
      // 获取未压缩文件列表
      bool NonCompressList(std::vector<std::string> *list)
      {
        // 遍历 _file_list 将没有压缩的文件名称添加到list中
        pthread_rwlock_rdlock(&_rwlock);
        auto it = _file_list.begin();
        for(;it != _file_list.end(); ++it)
        {
          if(it->first == it->second)
          {
            pthread_rwlock_unlock(&_rwlock);
            list->push_back(it->first);
          }
        }
        pthread_rwlock_unlock(&_rwlock);
        return true;
      }
      // 插入/更新数据
      bool Insert(const std::string &src, const std::string &dst)
      {
        // 更新修改需要加写锁
        pthread_rwlock_wrlock(&_rwlock);
        _file_list[src] = dst;
        pthread_rwlock_unlock(&_rwlock);
        // 更新修改之后, 重新备份
        Storage();
        return true;
      }
      // 获取所有文件名称,向外显示文件列表使用
      bool GetAllName(std::vector<std::string> *list)
      {
        // 加 读锁
        pthread_rwlock_rdlock(&_rwlock);
        auto it = _file_list.begin();
        for(; it != _file_list.end(); ++it)
        {
          // 获取的是源文件名称
          list->push_back(it->first);
        }
        pthread_rwlock_unlock(&_rwlock);
        return true;
      }
      // 数据改变后持久化存储
      // 存储的是管理的文件名数据
      bool Storage()
      {
        // 将 _file_list 中的数据进行持久化存储
        // 数据对象进行持久化存储-----序列化
        // 每个数据占据一行 每一行的格式: src dst\r\n
        std::stringstream tmp; // 实例化一个string流对象
        pthread_rwlock_rdlock(&_rwlock);
        auto it = _file_list.begin();
        for(; it != _file_list.end(); ++it)
        {
          // 序列化---按照指定格式组织数据
          tmp << it->first << " " << it->second << "\r\n";
        }
        pthread_rwlock_unlock(&_rwlock);
        // 将数据备份到文件中
        FileUtil::Write(_back_file, tmp.str());

        return true;
      }
      // 启动时初始化加载原有数据
      // 格式:  filename gzfilename\r\n filename gzfilename\r\n...
      bool InitLoad()
      {
        // 从数据的持久化存储文件中加载数据
        // 1. 将这个备份文件的数据读取出来
        std::string body;
        if(FileUtil::Read(_back_file, &body) == false)
        {
          return false;
        }

        // 2. 进行字符串处理, 按照/r/n 进行分割
        // boost::split(vector, src, sep, flag) 
        // vector: 分割后的字符串存放位置 src:源字符串 sep:遇到sep字符便分割
        std::vector<std::string> list;
        boost::split(list, body, boost::is_any_of("\r\n"), boost::token_compress_off);
        // 3. 每一行按照空格进行分割-----前面是key, 后面是val
        for(auto i : list)
        {
          size_t pos = i.find(" ");
          if(pos == std::string::npos)
          {
            continue;
          }
          std::string key = i.substr(0, pos);
          std::string val = i.substr(pos+1);
          
        
          // 4. 将 key/val 添加到 _file_list 中
          Insert(key, val);
        }
        return true;
      }
    private:
      // 持久化数据存储文件名称
      std::string _back_file; 
      // 数据管理器
      // 由于 _file_list 是一个临界资源,网络通信模块和数据管理模块都在使用,
      // 所以为了线程安全,需要在每次访问前加锁,访问结束后解锁
      std::unordered_map<std::string, std::string> _file_list;  
      pthread_rwlock_t _rwlock;
  };
  // 非热点文件压缩
  class NonHotCompress
  {
    public:
      NonHotCompress(const std::string dir_name):
        _gz_dir(dir_name)
      {
      }
      ~NonHotCompress()
      {

      }
      // 总体向外提供的功能接口, 开始压缩模块
      bool Start()
      {
        // 是一个循环的, 持续的的过程--每隔一段时间, 判断有没有非热点文件,然后进行压缩
        // 问题: 什么文件是非热点文件
        // ----当前时间减去最后一次访问时间 > 基准时间(我们自定义的一个阈值时间)
        while(1)
        {
          // 1.获取所有的未压缩文件列表
          std::vector<std::string> list;
          data_manage.NonCompressList(&list);
          // 2.逐个判断当前文件是否是热点文件
          for(int i = 0; i < list.size(); i++)
          {
            bool ret = FileIsHot(list[i]);
            if(ret == false)
            {
              // 源文件名称
              std::string src_name = BACKUP_DIR + list[i];
              std::string dst_name = GZFILE_DIR + list[i]; + ".gz";
            
              // 3.如果是非热点文件, 则压缩这个文件, 删除源文件
              if(CompressUtil::Compress(src_name, dst_name) == true)
              {
                // 更新数据信息
                data_manage.Insert();
                // unlink 删除一个文件的目录项
                unlink(src_name.c_str());
              }
            }
          }
          sleep(INTERVAL_TIME);
        }
        return true;
      }
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
