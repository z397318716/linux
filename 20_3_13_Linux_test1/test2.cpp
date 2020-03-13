#include<iostream>
#include<cmath>
using namespace std;

bool sushu(int x)
{
  if(x <= 1)
    return false;
  else if(x == 2)
    return true;
  else
  {
    int tmp = sqrt(x);
    for(int i = 2; i <= tmp; i++)
    {
      if(x % i == 0)
        return false;

    }

  }
  return true;

}
int main()
{
  int n;
  while(cin >> n)
  {
    if(n < 11)
    {
      cout << -1 << endl;

    }
    else
    {
      for(int i = 11; i < n; i += 10)
      {
        if(sushu(i))
          cout << i << ' ';

      }

    }



  }


  return 0;

}
