

#ifndef VTABLE_H_INCLUDED_
#define VTABLE_H_INCLUDED_

#include <vector>
#include "symbol.h"

class Vtable
{
private:
  std::vector<SymPtr> table;

public:
  int addVirtual(SymPtr funcSymbol)
  {
    table.push_back(funcSymbol);
    return ((int)table.size() - 1);
  }

  void addVirtual(SymPtr funcSymbol, int index)
  {
    assert(index < (int)table.size());
    table[index] = funcSymbol;
  }

  int getVirtualCount()
  {
    return ((int)table.size());
  }

  SymPtr getEntry(int index)
  {
    assert(index < (int)table.size());
    return table[index];
  }

};


#endif

