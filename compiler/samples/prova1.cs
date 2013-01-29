
#include  "./ciccio.h"

class pippo
{
public:

  int i;
  virtual int incr(int i, float f)
  {
    ++i;
  }

};

class pluto : pippo
{
public:

  int incr(int i, float f)
  {
    ++i;
    return i;
  }


  int incr(int j)
  {
    return(i += j);
  }

};

int main()
{
  pluto p;
  p.i = 1;

  if (false || true && p.i > 1)
  {
    p.i = 0;
  }
  else p.i = 10;

  for(int j = 2; j != 0; --j)
  {
    int xx = p.incr(1, 1.0 + 1.0);
    print((string)xx);
  }
  
  return 0;
}
