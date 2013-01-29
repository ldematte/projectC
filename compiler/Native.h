
#ifndef NATIVE_TABLE_H_INCLUDED
#define NATIVE_TABLE_H_INCLUDED


#include <vector>
#include <map>
#include <string.h>

#include "../Singleton.h"
#include "../NativeFunction.h"

#include "symbol.h"


class SysImportTable : public Singleton<SysImportTable>
{
private:
  std::map<SymPtr, NativeFunction> table;


public:

  NativeFunction& insert(SymPtr sym)
  {
    assert (sym->isNative);
    NativeFunction& nf = table[sym];
    nf.returnType = typeToTag(sym->type->getRetType());
    strcpy(nf.name, sym->name.c_str());
    
    std::vector<TypePtr> tmp = sym->type->getParamTypes();
    assert(tmp.size() == sym->type->getParamCount());    

    /*
    for (std::vector<TypePtr>::iterator it = tmp.begin(); it!= tmp.end(); ++it)
      nf.paramTypes.push_back(typeToTag(*it));
    */

    //for now, only cdecl
    nf.callType = CALL_CDECL;

    return nf;
  }

  NativeFunction& get(SymPtr sym)
  {
    return table[sym];
  }
};

#endif //NATIVE_TABLE_H_INCLUDED



