#include "output.hs"
#include "string.hs"

void fibonacci(int a[], int n)
{
  a[0] = 1;
  a[1] = 1;
  
  for(int i = 2; i < n; ++i)
  {
    a[i] = a[i-1] + a[i-2];  
  }

}


int main() 
{
  int num[10];
  
  fibonacci(num, 10);
  
  for(int i = 0; i < 10; ++i)
  {
    print((string)(num[i]));  
  }
}


