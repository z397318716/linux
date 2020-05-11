#include "cloud_backup.hpp"

// 压缩测试
void compress_test(char* argv[])
{
  // argv[1] = 源文件名称
  // argv[2] = 压缩包名称
  
  _cloud_sys::CompressUtil::Compress(argv[1], argv[2]);
  std::string file = argv[2];
  file += ".txt";
  _cloud_sys::CompressUtil::UnCompress(argv[2], file.c_str());
}


int main(int argc, char* argv[])
{
   
  _cloud_sys::DataManger data_manage("./test.txt");

  data_manage.InitLoad();
  data_manage.Insert("c.txt", "c.txt.gz");
  std::vector<std::string> list;
  // 获取所有信息测试
  data_manage.GetAllName(&list); // -----此处由于是输出型测试 list 必须加引用,否则报错
  for(auto i: list)
  {
    std::cout<< i << std::endl;
  }
  std::cout << "-------------" << std::endl;
  // 获取未压缩的文件名称测试
  list.clear();
  data_manage.NonCompressList(&list);

  for(auto i: list)
  {
    std::cout<< i << std::endl;
  }
  /*
  data_manage.Insert("a.txt", "a.txt");
  data_manage.Insert("b.txt", "b.txt.gz");
  data_manage.Insert("c.txt", "c.txt");
  data_manage.Insert("d.txt", "d.txt.gz");
  // 持久化存储
  data_manage.Storage();
  */




  return 0;
}
