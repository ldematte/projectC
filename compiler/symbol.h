
#ifndef SYMBOL_DEFINITION_H_INCLUDED_
#define SYMBOL_DEFINITION_H_INCLUDED_


#include "../common.h"
#include <string>
#include <list>
#include <vector>


#include "../type.h"
#include "Vtable.h"

#include "../constants.h"



enum SYMBOL_KIND
{
  SYM_CLASS,
  SYM_NAMESPACE,
  SYM_SCOPEBLOCK,
  SYM_FUNCTION,
  SYM_VAR,
  SYM_TYPEDEF
};

enum CLASS_FLAVOUR
{
  CLASS_CLASS,
  CLASS_UNION,
  CLASS_ENUM,
  CLASS_INTERFACE
};



//we need a symbol tree (see SymbolTable)

//class that holds a symbol definition
class SymDef
{
public:

  SymDef()
  {
    isVptrPresent = false; 
    isVptrIntroduced = false; 
    isAbstract = false; 
    isAllocated = true; 
    isAnonUnion = false; 
    isArgument = false; 
    isByRef = false;
    isConst = false; 
    isCtor = false; 
    isDefined = true; 
    isDerived = false; 
    isImplicit = false;
    isLocal = false; 
    isMember = false; 
    isOverload = false;
    isOverride = false;
    isStatic = false;
    isTagged = false;
    isVirtual = false;
    mustCheckInit = false;

    isFullyDeclared = false;
    isNative = false;

    //nextFunctionIndex = 0;
    //localNumber = 0;
    //paramNumber = 0;
    //localSize = 0;
    functionCodeSize = 0;
    //functionCount = 0;
    functionOffset = 0;

    type = NULL;
  }
  //the name of the symbol
  std::string name;

  //the type of the symbol
  TypePtr type;

  //the owner of this symbol (scope)
  SymPtr parent;

  //access level (public, private...)
  //for classes, the access level of the members
  ACCESS_LEVEL accessLevel;

  //symbol kind
  SYMBOL_KIND symKind;

  //e' definito? (ha un corpo) (per funzioni)
  bool isDefined;

  //la dichiarazione (che avviene in 2 fasi) e' stata completata?
  bool isFullyDeclared;

  //variables and functions
  bool isMember;
  bool isStatic;  

  //is created automatically by the compiler
  bool isImplicit;

  //next definition in hash table
  //SymPtr nextDef;              
  
  //next symbol in the same scope
  //SymPtr nextInScope;

  //the list of "sons" (symbols declared in this scope)
  std::list<SymPtr> declList;

  int addSymbol(SymPtr sym)
  {
    declList.push_back(sym);
    return (int)(declList.size() - 1);
  }
  
  //CLASS 
  
  //vtable
  Vtable vtable;

  //subkind of class, for now only class or namespace
  //(other types may be unions, interfaces and structs)
  CLASS_FLAVOUR classFlavour;

  //this class has virtual functions in it?
  //(in its declList?)
  bool isVptrPresent;

  //is the first class in the inheritance hierarchy to
  //have a virtual table?
  bool isVptrIntroduced;

  //contains a pure virtual function?
  bool isAbstract;

  //is it derived from another class?
  bool isDerived;      

  //if so, which is the father class?
  SymPtr father;

  //the number of funcrions within this scope
  int getFunctionCount()
  {
    int count = 0;
    std::list<SymPtr>::iterator it;
    for (it = declList.begin(); it != declList.end(); ++it)
      if ((*it)->symKind == SYM_FUNCTION)
        ++count;

    return count;
  }

  int getNativeFunctionCount()
  {
    int count = 0;
    std::list<SymPtr>::iterator it;
    for (it = declList.begin(); it != declList.end(); ++it)
      if ((*it)->symKind == SYM_FUNCTION && (*it)->isNative)
        ++count;

    return count;
  }

  int getMemberCount()
  {
    //Modificato per tener conto del numero di membri del padre
    int fatherMembers = 0;
    if (isDerived)
      fatherMembers = father->getMemberCount();

    int count = 0;
    std::list<SymPtr>::iterator it;
    for (it = declList.begin(); it != declList.end(); ++it)
      if ((*it)->symKind == SYM_VAR && (*it)->isMember == true)
        ++count;

    return fatherMembers + count;
  }

  //the DATA size of the object (excluding header)
  int getObjectSize()
  {
    return (getMemberCount() * WORD_SIZE);
  }

  //NAMESPACE
  //niente
  
  //SCOPEBLOCK
  int blockID;
  int beginAddress;
  int endAddress;

  //FUNCTION
  
  //the index of this function in the owner's vtable
  int vtableIndex;

  //the index of this function (the ordinal in the owner symbol)
  int functionIndex;

  bool isCtor :1;
  bool isVirtual :1;
  bool isOverride :1;
  bool isOverload :1;
  bool isNative;

  //the size of the object code executed by this function, in bytes
  int functionCodeSize;

  //the code begins at this location from the begin of the
  //text segment
  int functionOffset;

  int getParamCount()
  {
    int count = 0;
    std::list<SymPtr>::iterator it;
    for (it = declList.begin(); it != declList.end(); ++it)
      if ((*it)->symKind == SYM_VAR && (*it)->isArgument == true)
        ++count;

    return count;
  }

private:
 
  int _getLocalCountFunc()
  {
    int count = 0;
    std::list<SymPtr>::iterator it;
    for (it = declList.begin(); it != declList.end(); ++it)
    {
      if (((*it)->symKind == SYM_VAR) && ((*it)->isArgument == false))
        ++count;
      else if ((*it)->symKind == SYM_SCOPEBLOCK)
        count += (*it)->_getLocalCountFunc();
    }
    return (count);
  }

  int _getLocalCountSB()
  {
    SymPtr scope = this->parent;
    while (scope->symKind != SYM_FUNCTION)
    {
      //assert(scope != SymbolTable::GetSingleton().getGlobalNamespace())
      scope = scope->parent;
    }
    return scope->_getLocalCountFunc();
  }

public:

  //return the size occupied on the stack, in WORDS
  int getLocalCount()
  {
    //anche le variabili piu' complesse, come i riferimenti a oggetti,
    //occupano una WORD sullo stack

    //se il parent e' gia' una funzione
    if (this->symKind == SYM_FUNCTION)
    {
      return _getLocalCountFunc();      
    }
    else //if (this->symKind == SYM_SCOPEBLOCK)
    {
      return _getLocalCountSB();
    }
  }

  //VARIABLE  
  
  //local (auto) (including arguments)

  //FIX: non sarebbe meglio di no?
  bool isLocal :1;
  //local : is this an argument?
  bool isArgument :1; 
  //local : byref arg?
  bool isByRef :1;
  //static: space been allocated?
  bool isAllocated :1; 
  //compile time constant?
  bool isConst :1;
  //member of an anonymous union?
  bool isAnonUnion :1; 
  //member of a tagged union?
  bool isTagged :1; 
  //unitialized use possible?
  bool mustCheckInit :1;
  
  //number of this variable in class/function
  int varIndex;

  //the real offset from the beginnig of the stack 
  int getStackOffset()
  {
    assert(symKind == SYM_VAR);
    assert(isLocal || isArgument);

    if (isLocal)
      return varIndex + STACK_LOCALS_OFFSET;
    else //isArgument
      //return varIndex - parent->getParamCount() + 1 - STACK_PARAMS_OFFSET;
      //return -varIndex + STACK_PARAMS_OFFSET; //per averli girati dall'altra parte
      return STACK_PARAMS_OFFSET - (parent->getParamCount() - 1 - varIndex);
  }

  //the real absolute stack offset
  //valid ONLY for global variables!
  int getAbsStackOffset()
  {
    assert(symKind == SYM_VAR);
    assert(isGlobal());

    return varIndex;
  }

  bool isGlobal()
  {
    return (parent->name == "__global");
  }

  //the real offset from the begin address (this pointer)
  //of the class (in real memory bytes, including pointers
  //to the symbol table, tag for the types, ecc
  int getClassOffset()
  {
    return (varIndex * WORD_SIZE); //+ OBJDATA_HEADER_SIZE);
  }

  //TYPEDEF



private:
  
  //make a new symbol son of this one
  SymPtr addSymbol(const std::string& name, TypePtr type, ACCESS_LEVEL access, 
                   SYMBOL_KIND kind);
  SymPtr addSymbol(const std::string& name, ACCESS_LEVEL access, 
                   SYMBOL_KIND kind);

public:

  void applyModifiers(int modifiers);
  TypePtr makeType();
  //void updateType();

  //guarda se il simbolo dato quello dato e' nel mio scope
  //(fratello, figlio, o figlio di antenati)
  bool isInMyScope(SymPtr symbol);
  bool isSibiling(SymPtr symbol);
  bool isMyMember(SymPtr symbol, bool isPrivateAllowed);
  bool isMyAncestor(SymPtr symbol);

  SymPtr resolveSymbol(const std::string& name, const std::vector<TypePtr>& paramTypes);
  SymPtr bakerDitchfield(const std::string& name, const std::vector<TypePtr>& paramTypes);


  bool isTypename();
  bool isIdentifier();

  //non e' molto bello, dovrebbe funzionare ma forse e' il caso
  //di usare qualcosa di meglio
  bool isEqual(const SymPtr symbol)
  {
    return (this == symbol);
  }
  bool isEqual(const SymDef& symbol)
  {
    return ((parent == symbol.parent) && 
            (name == symbol.name)     && 
            (type == symbol.type));
  }

  void dump(int level = 0);

  std::string getQualifiedName();

  int computeSize()
  {
    int size = 0;
    std::list<SymPtr>::iterator i;
    for (i = declList.begin(); i != declList.end(); ++i)
    {
      SymPtr sym = *i;
      if (sym->symKind == SYM_VAR)
      {
        size = sym->type->getTypeSize();
      }
    }

    return size;
  }

};


#endif //SYMBOL_DEFINITION_H_INCLUDED_


