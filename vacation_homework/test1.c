#include<stdio.h>


int main(){
  int num, sum;
  num = 7, sum = 7;
  sum = num++;
  sum++;
  ++num;

  printf("%d\n", sum);
  int i = 10, j = 10, k = 3;
  k *= i + j;
  printf("%d\n", k);

  return 0;
}
