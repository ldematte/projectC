
#include "output.hs"
#include "string.hs"

class A
{
public:
  string nome;
  int n;
  
  string getName()
  {
    return stradd("Nome: ", nome);
  }
};

class B
{
public:
  A makeA()
  {
    return new A;
  }
};

int main()
{
  B b;
  string s = b.makeA().getName();
  
  print(s);
}
