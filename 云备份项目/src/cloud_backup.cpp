#include "cloud_backup.hpp"



int main(int argc, char* argv[])
{
  // argv[1] = 源文件名称
  // argv[2] = 压缩包名称
  

  _cloud_sys::CompressUtil::Compress(argv[1], argv[2]);
  std::string file = argv[2];
  file += ".txt";
  _cloud_sys::CompressUtil::UnCompress(argv[2], file.c_str());






  return 0;
}
