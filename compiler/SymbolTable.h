

#ifndef SYMBOL_TABLE_H_INCLUDED_
#define SYMBOL_TABLE_H_INCLUDED_

#include "common.h"
#include <Singleton.h>

#include "symbol.h"
#include "hash.h"
#include "MyStack.h"

#include <string>
#include <hash_map>
#include <vector>


//It is associated to an hash map for fast lookups.
class SymbolTable : public Singleton< SymbolTable >
{
public:

  SymbolTable() : nextId(0) { }

  //init: initialize the table, join the hash table
  //and create INTRINSICS types (global namespace, primitive types...)
  bool init();

  SymPtr declareSym(const std::string& name,
                             SYMBOL_KIND kind,
                             SymPtr parent);

  //declare a local scope 
  SymPtr declareLclBlock(SymPtr parent);

  //declare a nested class
  SymPtr declareNcs(const std::string& name, SymPtr parent);

  //declare an overloaded function
  SymPtr declareOvl(const std::string& name, SymPtr parent);

  //declare a class
  SymPtr declareClass(const std::string& name, SymPtr parent);

  //declare an inherited class
  SymPtr declareClass(const std::string& name, const std::string& father, SymPtr parent);
  
  //declare a namespace
  SymPtr declareNamespace(const std::string& name, SymPtr parent);

  //declare a function, can be a member one or not
  SymPtr declareFunction1(SymPtr new_symbol, TypePtr param_type);

  SymPtr declareFunction2(SymPtr new_symbol, TypePtr ret_type, ACCESS_LEVEL access);
  SymPtr declareNativeFunction2(SymPtr new_symbol, TypePtr ret_type, ACCESS_LEVEL access);

  SymPtr declareVirtualFunction(SymPtr new_symbol, TypePtr func_type, ACCESS_LEVEL access);

  SymPtr declareParam(const std::string& name, TypePtr param_type, SymPtr parent);

  SymPtr declareVar(SymPtr new_symbol, TypePtr var_type, SymPtr parent);

  SymPtr declareVar(const std::string& name, SymPtr parent);

  //search a symbol in the current scope, including class from which we derived
  bool isSymbolDeclared(SymPtr symbol, SymPtr& previous_symbol, SymPtr scope);

  bool isVarDeclared(SymPtr symbol, SymPtr& previous_symbol, SymPtr scope);

  bool hasFunctionParent(SymPtr parent1);

  //lookup a symbol in the specified namespace
  SymPtr lookupNSPSym();
  //lookup a symbol in the specified class
  SymPtr lookupClsSym();
  //lookup a symbol in the current scope
  SymDef& lookupScpSym();
  //lookup a symbol in the local scope
  SymPtr lookupLclSym();  
  //search a symbol in a class and in its parents
  SymPtr findInClass();

  //return the global namespace/scope
  SymPtr getGlobalNamespace() { return global; }

  //search if an identifier with name 'name' is already
  //defined
  bool lookupIdSymbol(const std::string& name);
  bool lookupIdSymbol(const std::string& name, Ident& id);

  //search if a typename called 'name' is already defined
  bool lookupTypeSymbol(const std::string& name);
  bool lookupTypeSymbol(const std::string& name, Ident& id);  

  //insert a symbol in our hashtable(s)
  void hashSymbol(SymPtr symbol);

  int setLastToken(const char* token) 
  { 
    tokens.push_back(token); 
    return ((int)tokens.size() - 1);
  }

  std::string getLexToken(int idx)
  { 
    return tokens[idx]; 
  }
  void clearTempTokens()
  {
    tokens.clear();
  }

  std::string getNextUnnamedId();

  void dump();

private:
  HashTab typenameHash;
  HashTab identHash;

  //our global namespace/scope
  SymPtr global;

  int nextId;

  std::vector<std::string> tokens;
};



#endif //SYMBOL_TABLE_H_INCLUDED_

