class A
{
public:
  void f()
  {
    ...
  }  
};

class B : public A
{
public:
  void f()
  {
    ...
  }  
  
  void g()
  {
    f();
    //B::f() hides A::f()
  }
};
