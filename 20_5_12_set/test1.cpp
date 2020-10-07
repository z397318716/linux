#include<set>
#include<iostream>

int main()
{

  int array[] = {1,3,5,7,9,2,4,6,8,10,3,5,9};
  std::set<int> s1(array,array+sizeof(array)/sizeof(array[0]));

  for(auto & e: s1)
  {
    std::cout << e << " " ;
  }
  std::cout << std::endl;

  std::set<int>::iterator it = s1.begin();
  for(; it != s1.end(); it++)
  {
    std::cout << *it << ' ' ;

  }
  std::cout << std::endl;
  std::cout << s1.count(1) << " " << s1.count(3) << std::endl;




  return 0;
}
