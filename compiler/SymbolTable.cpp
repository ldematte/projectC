//
// Implementation of the SymbolTable class
/////////////////////////////////////////////////

#include "SymbolTable.h"
#include "type.h"
#include "OutputManager.h"
#include "Native.h"
#include "ErrorManager.h"

#include <sstream>
#include <iostream>

#define OM OutputManager::GetSingleton()

bool SymbolTable::init()
{
  TypeRoot::GetSingleton().createStdTypes();

  global = SymPtr(new SymDef());
  global->accessLevel = ACCESS_PUBLIC;
  global->name = "__global";
  //global->parent.reset(); //= NULL;
  global->parent = global;
  global->symKind = SYM_NAMESPACE;
  global->type = TypeDef::createClass(global);

  hashSymbol(global);

  return true;
}

std::string SymbolTable::getNextUnnamedId()
{
  ++nextId;
  std::stringstream ss;

  ss << "__unnamed_" << nextId;
  return ss.str();
}

SymPtr SymbolTable::declareSym(const std::string& name,
                               SYMBOL_KIND kind,
                               SymPtr parent)
{
  SymPtr sym(new SymDef);
  sym->symKind = kind;
  sym->name = name;
  sym->parent = parent;

  sym->parent->declList.push_back(sym);
  hashSymbol(sym);
  return sym;
}


SymPtr SymbolTable::declareClass(const std::string& name, SymPtr parent)
{
  SymPtr new_symbol (new SymDef);

  new_symbol->classFlavour = CLASS_CLASS;
  new_symbol->isVptrPresent = false; //must be modified later, if necessary
  new_symbol->isVptrPresent = false;
  new_symbol->isDerived = false;
  new_symbol->father = NULL;
  new_symbol->isFullyDeclared = true;
  new_symbol->name = name;
  
  new_symbol->parent = parent;
  assert(parent->symKind == SYM_CLASS || parent->symKind == SYM_NAMESPACE);
  parent->declList.push_back(new_symbol);
 
  new_symbol->symKind = SYM_CLASS;
  new_symbol->type = TypeDef::createClass(new_symbol);

  //sto dichiarando un tipo
  parent->type->addType(new_symbol->type);

  //for now, all classes are public
  new_symbol->accessLevel = ACCESS_PUBLIC;
  hashSymbol(new_symbol);

  return new_symbol;
}

SymPtr SymbolTable::declareClass(const std::string& name, const std::string& father, SymPtr parent)
{
  SymPtr sym = declareClass(name, parent);
  sym->isDerived = true;

  Ident idFather;
  bool success = typenameHash.lookupString(father, idFather);
  if (!success)
  {
    //raise error
    std::string s = "Base class \"" + name + "\" is not defined";
    ErrorManager::GetSingleton().logAndRaise(s.c_str());
  }

  //deve essere nello scope dello stesso blocco (parent)
  //o in blocchi piu' bassi, mai in blocchi figli o fratelli
  bool found = false;
  std::list<SymPtr>::iterator i;
  for (i = idFather.symDefList.begin(); i != idFather.symDefList.end(); ++i)
    if(sym->isSibiling(*i))
    {
      sym->father = *i;
      found = true;
      //ho trovato mio padre
      //ora devo vedere se devo copiarmi la vtable
      if (sym->father->isVptrPresent)
      {
        sym->vtable = sym->father->vtable;
        sym->isVptrPresent = true;
      }
      break;
    }
  if (!found)
  {
    //raise error
    std::string s = "\"" + name + "\" is not a class symbol or is not accessible";
    ErrorManager::GetSingleton().logAndRaise(s.c_str());

  }        
  return sym;  
}

SymPtr SymbolTable::declareNamespace(const std::string& name, SymPtr parent)
{
  assert(parent->symKind == SYM_NAMESPACE);

  SymPtr new_symbol(new SymDef);

  //namespaces are always public
  new_symbol->accessLevel = ACCESS_PUBLIC;
  new_symbol->name = name;
  new_symbol->parent = parent;
  new_symbol->symKind = SYM_NAMESPACE;
  new_symbol->isFullyDeclared = true;

  //?? e' corretto?
  new_symbol->type = TypeDef::createClass(new_symbol);

  parent->declList.push_back(new_symbol);
  hashSymbol(new_symbol);

  return new_symbol;
}

SymPtr SymbolTable::declareLclBlock(SymPtr parent)
{
  assert(/*parent->symKind == SYM_CLASS ||*/
         parent->symKind == SYM_SCOPEBLOCK ||
         parent->symKind == SYM_FUNCTION);

  SymPtr new_symbol(new SymDef);

  //blocks are always public
  new_symbol->accessLevel = ACCESS_PUBLIC;
  new_symbol->name = "__unnnamed" + this->getNextUnnamedId();
  new_symbol->parent = parent;
  new_symbol->symKind = SYM_SCOPEBLOCK;
  new_symbol->blockID = this->nextId;
  new_symbol->isFullyDeclared = true;

  new_symbol->type = TypeDef::createClass(new_symbol);

  parent->declList.push_back(new_symbol);
  return new_symbol;
}

SymPtr SymbolTable::declareFunction1(SymPtr new_symbol, TypePtr param_type)
{
  assert(new_symbol->parent->symKind == SYM_NAMESPACE ||
         new_symbol->parent->symKind == SYM_CLASS);

  new_symbol->symKind = SYM_FUNCTION;
  new_symbol->type = param_type;

  //l'ultimo simbolo dichiarato e' quello della nostra funzione
  assert(new_symbol->isEqual(new_symbol->parent->declList.back()));
  new_symbol->functionIndex = new_symbol->parent->getFunctionCount();
  new_symbol->isFullyDeclared = true;
  
  return new_symbol;
}

SymPtr SymbolTable::declareFunction2(SymPtr new_symbol, TypePtr ret_type, ACCESS_LEVEL access)
{
  assert(new_symbol->parent->symKind == SYM_NAMESPACE ||
         new_symbol->parent->symKind == SYM_CLASS);

  new_symbol->accessLevel = access;
  //add return type
  new_symbol->type->addType(ret_type);
  new_symbol->isFullyDeclared = true;
  
  if (new_symbol->parent->symKind == SYM_CLASS)
  {
    new_symbol->isMember = true;
    
    //vediamo se nello scope della classe parent (lei e i suoi antenati)
    //il simbolo e' gia' dichiarato
    SymPtr previous_decl;
    if (isSymbolDeclared(new_symbol, previous_decl, new_symbol->parent))
    {
      if (previous_decl->symKind == SYM_FUNCTION)
      {
        //overload: il simbolo appartiene alla stessa classe, i tipi sono diversi
        if (previous_decl->parent->isEqual(new_symbol->parent))
        {
          if (previous_decl->type->sameParams(new_symbol->type))
          {
            //raise error
            std::string s = "Function \"" + new_symbol->name + "\" redeclaration ";
            ErrorManager::GetSingleton().logAndRaise(s.c_str());
          }
          else
          {
            //si tratta di un overload
            previous_decl->isOverload = true;
            new_symbol->isOverload = true;
            OM.out(OutputManager::INFO, "function %s overloaded", new_symbol->name.c_str());
          }
        }
        else
        {
          if (new_symbol->parent->isDerived) 
          {
            //la classe corrente e' derivata e non e' la stessa
            //cioe', sto ridichiarando in una figlia
            if (previous_decl->type->isEqual(new_symbol->type))
            {
              //ok, this is an override
              //it is virtual or not?
              if (previous_decl->isVirtual)
              {
                //here is where we update the vtable
                new_symbol->isVirtual = true;
                OM.out(OutputManager::INFO, "function %s [%s] virtual override", new_symbol->name.c_str(), new_symbol->type->getName().c_str());
                new_symbol->vtableIndex = previous_decl->vtableIndex;
                new_symbol->parent->vtable.addVirtual(new_symbol, new_symbol->vtableIndex);
              }
              else
              {
                //do nothing special
                OM.out(OutputManager::INFO, "function %s override", new_symbol->name.c_str());
              }
            }
            else
            {
              //it could be an overload
              //but are we going to hide?
              if (!previous_decl->parent->isEqual(getGlobalNamespace()))
              {
                OM.out(OutputManager::WARNING, "function %s override, could lead to function hiding", new_symbol->name.c_str());
              }
              //else the previous is in global namespace
              //do nothing special
            }
          }
        }
      }
      else
      {
        //raise error: name already used (and not a function)
        std::string s = "Symbol \"" + new_symbol->name + "\" already used (and not a function)";
        ErrorManager::GetSingleton().logAndRaise(s.c_str());
      }
    }
  }

  //gia' fatto da declareVar  
  //parent->declList.push_back(new_symbol);
  //hashmap.hashString(name, new_symbol);

  return new_symbol;
}

SymPtr SymbolTable::declareNativeFunction2(SymPtr new_symbol, TypePtr ret_type, ACCESS_LEVEL access)
{
  SymPtr funcSymbol = declareFunction2(new_symbol, ret_type, access);
  funcSymbol->isNative = true;

  /*NativeFunction& nf =*/ SysImportTable::GetSingleton().insert(funcSymbol);
  //TODO: set calling convention

  return funcSymbol;
}


bool SymbolTable::isSymbolDeclared(SymPtr symbol, SymPtr& previous_symbol, SymPtr scope)
{
  //cerca in scope
  bool found = false;

  std::list<SymPtr>::iterator i;
  for (i = scope->declList.begin(); i != scope->declList.end(); ++i)
  {
    if ( ((*i)->name == symbol->name) && !(symbol->isEqual(*i)) )
    {
      previous_symbol = *i;
      return true;
    }
  }

  //ripeti la ricerca ricorsivamente  
  if (scope->symKind == SYM_SCOPEBLOCK)
  {
    found = isSymbolDeclared(symbol, previous_symbol, scope->parent);
  }  
  else if (scope->symKind == SYM_CLASS)
  {
    //cerco nelle classi antenate
    if (scope->isDerived)
      found = isSymbolDeclared(symbol, previous_symbol, scope->father);

    //ma non solo, anche nei "contenitori" (classi outer)
    if (!found)
      found = isSymbolDeclared(symbol, previous_symbol, scope->parent);
  }
  else if (scope->symKind == SYM_NAMESPACE)
  {
    //fermati se tocchiamo il fondo (global namespace)
    if (scope->isEqual(SymbolTable::GetSingleton().getGlobalNamespace()))
      return false;
    else
      found = isSymbolDeclared(symbol, previous_symbol, scope->parent);
  }

  return found;
}

bool SymbolTable::isVarDeclared(SymPtr symbol, SymPtr& previous_symbol, SymPtr scope)
{
  return isSymbolDeclared(symbol, previous_symbol, scope);
}

SymPtr SymbolTable::declareVirtualFunction(SymPtr new_symbol, TypePtr func_type, ACCESS_LEVEL access)
{
  assert(new_symbol->parent->symKind == SYM_CLASS);

  SymPtr func_sym = declareFunction2(new_symbol, func_type, access);
  SymPtr parent = func_sym->parent;

  //must always be a member
  new_symbol->isMember = true;
  new_symbol->isVirtual = true;
  new_symbol->isFullyDeclared = true;

  if (!parent->isVptrPresent)
  {
    //sono il primo della gerarchia ad avere vtable
    parent->isVptrPresent = true;
    parent->isVptrIntroduced = true;
  }

  //build the vtable
  new_symbol->vtableIndex = parent->vtable.addVirtual(new_symbol);
  
  //gia' fatto da declareVar
  //parent->declList.push_back(new_symbol);
  //hashmap.hashString(name, new_symbol);
  

  return new_symbol;
}

SymPtr SymbolTable::declareParam(const std::string& name, TypePtr param_type, SymPtr parent)
{
  assert(parent->symKind == SYM_FUNCTION);

  SymPtr new_symbol = new SymDef;

  //locals and parameters are always public
  new_symbol->accessLevel = ACCESS_PUBLIC;
  new_symbol->name = name;
  new_symbol->parent = parent;
  new_symbol->symKind = SYM_VAR;
  new_symbol->type = param_type;
  new_symbol->isDefined = true;
  new_symbol->isArgument = true;
  new_symbol->isFullyDeclared = true;
  new_symbol->isByRef = param_type->isReference;
  new_symbol->varIndex = parent->getParamCount(); //niente -1, il parametro viene aggiunto
                                                  //proprio la riga successiva
  //while (parent->symKind == SYM_SCOPEBLOCK)
  parent->declList.push_back(new_symbol);
  hashSymbol(new_symbol);
 
  return new_symbol;
}

bool SymbolTable::hasFunctionParent(SymPtr parent1)
{
  if (parent1->symKind == SYM_FUNCTION)
    return true;
  else if (parent1->symKind == SYM_SCOPEBLOCK)
    return hasFunctionParent(parent1->parent);
  else
    return false;
}

SymPtr SymbolTable::declareVar(SymPtr new_symbol, TypePtr var_type, SymPtr parent)
{
  //locals and parameters are always public
  new_symbol->parent = parent;
  new_symbol->symKind = SYM_VAR;
  new_symbol->type = var_type;
  new_symbol->isDefined = true;
  new_symbol->isFullyDeclared = true;

  if (hasFunctionParent(new_symbol->parent))
  {
    new_symbol->isLocal  = true;
    new_symbol->varIndex = parent->getLocalCount() - 1;
  }
  else if (new_symbol->parent->symKind == SYM_CLASS)
  {
    new_symbol->isMember = true;
    new_symbol->varIndex = parent->getMemberCount() - 1;
  }
  else //global namespace
  {
    new_symbol->isMember = true;
    new_symbol->varIndex = parent->getMemberCount() - 1;
  }
  
  return new_symbol;
}

SymPtr SymbolTable::declareVar(const std::string& name, SymPtr parent)
{
  SymPtr new_symbol(new SymDef);

  new_symbol->accessLevel = ACCESS_PUBLIC;
  new_symbol->name = name;
  new_symbol->parent = parent;
  new_symbol->symKind = SYM_VAR;
  //new_symbol->isFullyDeclared = false;

  parent->declList.push_back(new_symbol);
  hashSymbol(new_symbol);

  return new_symbol;
}

bool SymbolTable::lookupIdSymbol(const std::string& name)
{
  Ident id;
  return identHash.lookupString(name, id);
}

bool SymbolTable::lookupIdSymbol(const std::string& name, Ident& id)
{
  return identHash.lookupString(name, id);
}

bool SymbolTable::lookupTypeSymbol(const std::string& name)
{
  Ident id;
  return typenameHash.lookupString(name, id);
}

bool SymbolTable::lookupTypeSymbol(const std::string& name, Ident& id)
{
  return typenameHash.lookupString(name, id);
}

void SymbolTable::hashSymbol(SymPtr symbol)
{
  if (symbol->isTypename())
    typenameHash.hashString(symbol->name, symbol);
  else if (symbol->isIdentifier())
    identHash.hashString(symbol->name, symbol);
  else 
    //we are trying to hash something wrong
    assert(false);
}

void SymbolTable::dump()
{
  std::cout << std::endl;
  std::cout << "================" << std::endl;
  std::cout << "| SYMBOL TABLE |" << std::endl;
  std::cout << "================" << std::endl;
  std::cout << std::endl;
  global->dump();
  std::cout << std::endl;
  std::cout << " TYPE HASH " << std::endl;
  std::cout << "-----------" << std::endl;
  std::cout << std::endl;
  typenameHash.dump();

  std::cout << std::endl;
  std::cout << " IDENTIFIER HASH " << std::endl;
  std::cout << "-----------------" << std::endl;
  std::cout << std::endl;
  identHash.dump();


}

SymbolTable SymbolTableInstance;

