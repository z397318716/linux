#include<iostream>

int func(int a){
  int b;
  switch(a){
    case 1:b = 30;
    case 2:b = 20;
    case 3:b = 16;
    case 4:b = 8;
    default: b = 0;
  }
  return b;
}

int main(){
  //std::cout << func(1) << std::endl;
  int a[3][3] = {0};
  int tmp = *(*a+1)+2;
  int tmp2 = (*(a+1))[2];

  std::cout<< tmp << std::endl;
  std::cout<< tmp2 << std::endl;

  return 0;
}

