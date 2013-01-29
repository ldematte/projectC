//
// Implementation of the SymDef class
////////////////////////////////////////

#include "symbol.h"
#include "ScopeManager.h"
#include "SymbolTable.h"
#include "OutputManager.h"
#include "ErrorManager.h"

#include <iostream>
#include <string>

#define SM ScopeManager::GetSingleton()
#define ST SymbolTable::GetSingleton()
#define OM OutputManager::GetSingleton()

TypePtr SymDef::makeType()
{
  //TypePtr ParentType = this->parent->type;
  TypePtr type(new TypeDef);

  switch(symKind)
  {
  case SYM_CLASS:
  case SYM_NAMESPACE:
  case SYM_SCOPEBLOCK:
    type->typeID = TYPE_CLASS;
    break;

  case SYM_FUNCTION:
    type->typeID = TYPE_PROD;
    break;

  case SYM_VAR:
    //come conosciamo il tipo giusto?
    //dovrebbe gia' essere nella type table...
    //o magari verra' settato dal chiamante
    break;

  case SYM_TYPEDEF:
    //di chi sono il typedef? devo agganciarmi al tipo giusto
    break;

  default:
    type->typeID = TYPE_VOID;
    break;
  }

  return type;
}




//BakerDitchfield( this_name : name, args : list< set< tree > > ) : tree_list
//  result_trees : set< tree > = {}
//  for each interpretation (return_type, formals) corresponding to this_name
//    if formals.length == args.length then
//      new_tree : tree
//      new_tree.type = return_type
//      new_tree.cost = 0
//      for i = 1 to formals.length
//        if there exists j in args[i] such that j.type can be converted to formals[i].type then
//          find k in args[i] such that conversion_cost( k.type, formals[i].type ) + k.cost is minimized
//          new_tree.child[i] = k
//          new_tree.cost += k.cost
//          new_tree.cost += conversion_cost( k.type, formals[i].type )
//        else
//          skip to next interpretation
//          (i.e. this interpretation is not consistent with the possible argument types)
//	end if
//      end for
//      if this interpretation is valid then
//        if there exists i in result_trees such that i.type == new_tree.type then
//          if i.cost == new_tree.cost then
//            i.ambiguous = true
//          else if i.cost < new_tree.cost then
//            remove i from result_trees
//            add new_tree to result_trees
//          (else throw away new_tree)
//	        end if
//        else
//          add new_tree to result_trees
//	      end if
//      end if
//    end if
//  end for
//  for each tree in result_trees
//    if tree.ambiguous then
//      remove tree from result_trees
//    end if
//  end if
//  return result_trees
//
SymPtr SymDef::bakerDitchfield(const std::string& name, const std::vector<TypePtr>& paramTypes)
{
  Ident id;
  bool success = ST.lookupIdSymbol(name, id);
  if (!success)
  {
    //raise error
    std::string s = "Function \"" + name + "\" not found in current scope (" + name + ")";
    ErrorManager::GetSingleton().logAndRaise(s.c_str());
  }

  SymPtr solution = NULL;
  int solution_cost = 0;
  bool valid = true;
  bool ambiguos = false;

  //for each interpretation (return_type, formals) corresponding to this_name
  std::list<SymPtr>::iterator i;
  for (i = id.symDefList.begin(); i != id.symDefList.end(); ++i)
  {
    SymPtr current_symbol = *i;
    if (current_symbol->symKind == SYM_FUNCTION && isInMyScope(current_symbol))
    {
      //if formals.length == args.length then
      if (current_symbol->type->getParamCount() == paramTypes.size())
      {
        int current_cost = 0;
        valid = true;
        std::deque<TypePtr>::iterator f_it = current_symbol->type->typeList.begin();
        std::vector<TypePtr>::const_iterator a_it = paramTypes.begin();

        //for i = 1 to formals.length
        for (; a_it != paramTypes.end(); ++a_it, ++f_it)
        {
          //if there exists j in args[i] such that j.type can be converted to formals[i].type then
          if ((*a_it)->isCompatible(*f_it))
          {
            //find k in args[i] such that conversion_cost( k.type, formals[i].type ) + k.cost is minimized
            //cioe' trova quello con costo minimo
            current_cost += (*a_it)->getCost(*f_it);            
          }
          else 
          {
            //questa interpretazione non e' valida
            //skip to next interpretation
            //(i.e. this interpretation is not consistent with the possible argument types)

            valid = false;
            break;
          }
        }

        if (valid)
        {
          if (solution == NULL)
          {
            solution = current_symbol;
            solution_cost = current_cost;
          }
          else
          {
            //if i.cost == new_tree.cost then
            if (solution_cost == current_cost)
            {
              //se e' in una classe derivata (overload)...

              //altrimenti
              ambiguos = true;
              return NULL;
            }          
            //else if i.cost < new_tree.cost then
            else if (solution_cost < current_cost)
            {
              //remove i from result_trees
              //add new_tree to result_trees
              solution = current_symbol;
              solution_cost = current_cost;
            }
            //(else throw away new_tree)
          }
        }
      }
    }
  }
  if (valid && !ambiguos && solution)
    return solution;
  else
  {
    std::string s = "No valid match for function \"" + name + "\" in current scope (" + name + ")";
    ErrorManager::GetSingleton().logAndRaise(s.c_str());

    return NULL;
  }
}

SymPtr SymDef::resolveSymbol(const std::string& name, const std::vector<TypePtr>& paramTypes)
{
  Ident id;
  bool success = ST.lookupIdSymbol(name, id);
  if (!success)
  {
    //raise error
    std::string s = "Symbol \"" + name + "\" not found in current scope (" + name + ")";
    ErrorManager::GetSingleton().logAndRaise(s.c_str());
  }

  SymPtr currentScope;
  if (symKind == SYM_VAR && type->typeID == TYPE_CLASS)
    currentScope = type->getSymbol();
  else
    currentScope = this;
  std::list<SymPtr>::iterator i;
  for (i = id.symDefList.begin(); i != id.symDefList.end(); ++i)
  {
    if(currentScope->isInMyScope(*i))
    {
      return (*i);
    }
  }

  //if we arrived here, raise error
  std::string s = "No valid match for symbol \"" + name + "\" in current scope (" + name + ")";
  ErrorManager::GetSingleton().logAndRaise(s.c_str());

  return NULL;
}

bool SymDef::isMyMember(SymPtr symbol, bool isPrivateAllowed)
{
  bool privateAccess = isPrivateAllowed;
  SymPtr current_class = this;
  
  for (;;)
  {
    std::list<SymPtr>::iterator i;  
    for (i = current_class->declList.begin(); i != current_class->declList.end(); ++i)
    {
      if ((*i)->isEqual(symbol))
      {
        if (privateAccess == false && symbol->accessLevel == ACCESS_PRIVATE)
        {
          //raise error
          std::string s = "Tying to access private symbol \"" + symbol->name + "\"";
          ErrorManager::GetSingleton().logAndRaise(s.c_str());
        }
        else
        {
          OM.out(OutputManager::INFO, "Found derived symbol %s from %s", symbol->getQualifiedName().c_str(), getQualifiedName().c_str());
          return true;
        }
      }
      else if ((*i)->name == symbol->name)
      {
        //stesso nome/tipo piu' in su nella gereachia
        //il simbolo non e' visibile
        if (symbol->symKind == SYM_FUNCTION)
        {
          if (symbol->type->sameParams((*i)->type))
            return false;
          //else continua la ricerca
        }
        else
        {
          //non e' permessa la ridefinizione di variabili
          return false;
        }
      }
    }

    if (current_class->isDerived)
    {
      current_class = current_class->father;
      //passato il primo livello, non si fa altro
      privateAccess = false;
    }
    else
      break;
  }

  return false;
}

bool SymDef::isInMyScope(SymPtr symbol)
{
  //se e' fratello di (per esempio) mio padre, non vale
  bool privateAccess = true;
  SymPtr global = ST.getGlobalNamespace();

  SymPtr current_scope;
  if (symKind == SYM_VAR && type->typeID == TYPE_CLASS)
  {
    current_scope = type->getSymbol();
    //non posso accedere ai privati
    privateAccess = false;
  }
  else
    current_scope = this;

  for (;;)
  {
    std::list<SymPtr>::iterator i;

    //se sto esaminando uno scope (classe) derivato, allora 
    //procedi

    //ma se ho lo stesso nome/stesso tipo ridefinito in un figlio
    //allora non e' visibile
    if (current_scope->isDerived)
    {
      //TODO: basta?
      if (current_scope->isMyMember(symbol, privateAccess))
        return true;
    }
    else
    {
      for (i = current_scope->declList.begin(); i != current_scope->declList.end(); ++i)
        if ((*i)->isEqual(symbol))
          return true;
    }

    if (current_scope->isEqual(global))
      break;

    current_scope = current_scope->parent;
  } 

  return false;
}

bool SymDef::isSibiling(SymPtr symbol)
{
  std::list<SymPtr>::iterator i;

  for (i = parent->declList.begin(); i != parent->declList.end(); ++i)
    if ((*i)->isEqual(symbol))
      return true; 

  return false;
}

bool SymDef::isMyAncestor(SymPtr symbol)
{
  SymPtr current = this;
  while(current->isDerived)
  {
    if (current->father->isEqual(symbol))
      return true;
    current = current->father;
  }
  return false;
}

bool SymDef::isTypename()
{
  return (this->symKind == SYM_CLASS || 
          this->symKind == SYM_NAMESPACE ||
          this->symKind == SYM_TYPEDEF);
}

bool SymDef::isIdentifier()
{
  return (this->symKind == SYM_FUNCTION || 
          this->symKind == SYM_VAR);
}


void SymDef::dump(int level)
{
  std::string header;
  for (int j = 0; j < level; ++j)
    header += "-";
  header += "> ";

  std::cout << header << this->name << " (parent: " << parent->name << ", ";
  std::cout << "type: " << type->getName() << ")" << std::endl;
  std::list<SymPtr>::iterator i;
  for (i = declList.begin(); i != declList.end(); ++i)
    (*i)->dump(level + 1);
}

std::string SymDef::getQualifiedName()
{
  std::string s(name);
  SymPtr current = parent;
  while (current != SymbolTable::GetSingleton().getGlobalNamespace())
  {
    s = current->name + "::" + s;
    current = current->parent;
  }

  return s;
}

