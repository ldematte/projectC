
#ifndef HASH_TABLE_H_INCLUDED_
#define HASH_TABLE_H_INCLUDED_

#include "../common.h"
#include "symbol.h"

#include <string>
#include <hash_map>
#include <map>
#include <list>
#include <iostream>

class Ident
{
public:

  //list of definitions, if any
  std::list<SymPtr> symDefList;       

  //see IDF_XXXX below
  byte flags;        

  std::string& getSpelling() { return name; }
  std::string name;  

  void addSymbol(SymPtr symbol) { symDefList.push_back(symbol); }

};

//the identifier is defined as a macro?
const int IDF_MACRO  = 0x01; 
//the identifier was invented by compiler
const int IDF_HIDDEN = 0x02;  
//identifier referenced by source code
const int IDF_USED   = 0x04;          


class HashTab
{
private:

  typedef std::hash_map< std::string, Ident > IDENT_MAP;
  IDENT_MAP IdentMap;

public: 

  void hashString(const std::string& name, SymPtr symbol)
  {
    Ident& id = IdentMap[name];
    id.addSymbol(symbol);
    id.name = name;
  }

  bool lookupString(const std::string& name, Ident& id) const
  {
    IDENT_MAP::const_iterator res;
    res = IdentMap.find(name);
    if (res != IdentMap.end())
    {
      id = (*res).second;
      return true;
    }
    else return false;
  }

  void dump() 
  {
    IDENT_MAP::iterator i;
    for (i = IdentMap.begin(); i != IdentMap.end(); ++i)
    {
      std::list<SymPtr>::iterator j;
      Ident& id = (*i).second;
      std::cout << id.name << ": ";
      for (j = id.symDefList.begin(); j != id.symDefList.end(); ++j)
        std::cout << (*j)->parent->name << "::" << id.name << "; ";
      std::cout << std::endl;
    }
  }
};

#endif //HASH_TABLE_H_INCLUDED_


