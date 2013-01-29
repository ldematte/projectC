
#include "ScopeManager.h"
#include "SymbolTable.h"
#include "type.h"

void ScopeManager::init()
{
  scopeStack_.clear();
  scopeStack_.push(SymbolTable::GetSingleton().getGlobalNamespace());
}

void ScopeManager::pushScope(SymPtr new_symbol)
{
  scopeStack_.push(new_symbol);
}


void ScopeManager::popScope()
{
  scopeStack_.pop();
}

SymPtr ScopeManager::getCurrentScope()
{
  return scopeStack_.top();
}


//Our singleton instance
ScopeManager ScopeManagerSingleton;

