#include<boost/algorithm/string.hpp>
#include<iostream>
#include<string>
#include<vector>

int main()
{

  std::string src = "ni!hao!a!xiao peng you";
  std::vector<std::string> list;
  boost::split(list, src,boost::is_any_of("!"), boost::token_compress_on);


  for(auto e : list)
  {
    std::cout << e << std::endl;
  }
  return 0;
}
