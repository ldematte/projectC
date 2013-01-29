
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
private:
  A a;
public:
  A makeA()
  {
    return new A;
  }
  
  void setName(string name)
  {
    a.nome = name;
  }
  
  A returnA()
  {
    return a;
  }
};

int main()
{
  B b;
  b.setName("Pippo");
  string s = b.returnA().getName();
  
  print(s);
}
