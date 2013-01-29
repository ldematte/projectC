
#ifndef SCOPE_STACK_H_INCLUDED_
#define SCOPE_STACK_H_INCLUDED_

#include "../common.h"
#include <Singleton.h>
#include <string>
#include "MyStack.h"
#include "symbol.h"

class ScopeManager : public Singleton<ScopeManager>
{
public:
  ScopeManager() { }
  void init();

  void pushScope(SymPtr new_symbol);
  void popScope();

  SymPtr getCurrentScope();

private:
  MyStack<SymPtr> scopeStack_;
};

#endif //SCOPE_STACK_H_INCLUDED_


