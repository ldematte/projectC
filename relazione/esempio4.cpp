class C
{
public: 
  int c;
}

class B
{
public:
   C a();
}

int main()
{
  A b;
  b.a().c = 1;
  print((string)b.a().c);
}

