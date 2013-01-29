//
// Implemention of type classes
//////////////////////////////////////////

#include "type.h"
#include "symbolP.h"
#include "Operand.h"
#include "compiler/SymbolTable.h"
#include "compiler/ErrorManager.h"

//
// TypeDef class
//////////////////////////////////////////

//costo di conversione

//costs[string][bool] da stringa a bool
/*
//to                 int float string bool object
int costs[5][5] = { { 0,    5,    9,    1,    9 },   //int     from
                    { 2,    0,    9,    9,    9 },   //float
                    { 6,    6,    0,    6,    9 },   //string
                    { 1,    9,    9,    0,    9 },   //bool
                    { 4,    4,    4,    4,    1 } }; //object
*/
//from               int float string bool object
int costs[5][5] = { { 0,    2,    6,    1,    4 },   //int     to
                    { 5,    0,    6,    9,    4 },   //float
                    { 9,    9,    0,    9,    4 },   //string
                    { 1,    9,    6,    0,    4 },   //bool
                    { 9,    9,    9,    9,    1 } }; //object

const int AUTOMATIC_THRES = 5;
const int INFINITE_THRES = 9;

TypePtr TypeDef::createArray(TypePtr arrayOf, int dim)
{
  TypePtr type = createType(TYPE_ARRAY);
  //il nome viene composto in getName
  //type->name = arrayOf->name + "[]";
  type->addType(arrayOf);
  type->arraySize = dim;
  return type;
}

TypePtr TypeDef::createProduct()
{
  TypePtr type = createType(TYPE_PROD);
  return type; 
}

TypePtr TypeDef::createClass(SymPtr symbol)
{
  TypePtr type = createType(TYPE_CLASS);
  type->symbol = symbol;
  type->name = symbol->name;
  return type; 
}

TypePtr TypeDef::createType(TYPE_TYPE typeID)
{
  TypePtr type(new TypeDef);
  type->typeID = typeID;
  switch (typeID)
  {
  case TYPE_INT:
    type->name = "int";
    break;
  case TYPE_FLOAT:
    type->name = "float";
    break;
  case TYPE_STRING:
    type->name = "string";
    break;
  case TYPE_VOID:
    type->name = "void";
    break;
  case TYPE_BOOL:
    type->name = "bool";
    break;
  default:
    break;
  }
  return type;
}

TypePtr TypeDef::createEmptyArray()
{
  TypePtr type = createType(TYPE_ARRAY);
  return type;
}

TypePtr TypeDef::createEmptyArray(int dim)
{
  assert(dim > 0);
  TypePtr type = createType(TYPE_ARRAY);
  type->arraySize = dim;
  return type;
}

TypePtr TypeDef::createParameter(TypePtr type, bool byVal, bool isArray)
{
  if (byVal)
  {
    if (isArray)
      return createArray(type, -1);
    else
      return type;
  }
  else
  {
    TypePtr paramType(type);
    paramType->isReference = true;
    if (isArray)
      return createArray(paramType, -1);
    else
      return paramType;
  }
}

TypePtr TypeDef::createConst(TypePtr type)
{
  TypePtr constType(type);
  constType->isConst = true;
  return constType;
}

TypePtr TypeDef::createPointer(TypePtr type)
{
  TypePtr pointerType(type);
  pointerType->isPointer = true;
  return pointerType;
}

TypePtr TypeDef::createError()
{
  TypePtr type = createType(TYPE_ERR);
  return type;
}

std::string TypeDef::getName()
{
  std::string typeName;
  if (typeID == TYPE_ARRAY)
    typeName = this->getArrayElemType()->getName() + "[]";
  else if (typeID == TYPE_CLASS)
    typeName = "class " + name;
  else if (typeID == TYPE_PROD)
  {
    typeName = "(";
    if (typeList.size() == 0)
      typeName = "() -> void";
    else if (typeList.size() == 1)
      typeName = "() -> " + typeList[0]->getName();
    else
    {
      size_t i;
      for (i = 0; i < typeList.size() - 2; ++i)
        typeName += (typeList[i]->getName() + " x ");

      typeName += (typeList[i]->getName() + ")");
      typeName += (" -> " + typeList[i + 1]->getName());
    }
  }
  else
    typeName = name;
  return typeName;
}

bool TypeDef::isError()
{ 
  return isEqual(TypeRoot::GetSingleton().getErr());
}

int TypeDef::getTypeSize()
{ 
  switch (typeID)
  {
    /*
  case TYPE_BOOL:
    return sizeof(bool);
  case TYPE_FLOAT:
    return sizeof(float);
    */
  case TYPE_CLASS:
    {
      int dim = 0;
      std::deque<TypePtr>::iterator i;
      for (i = typeList.begin(); i!= typeList.end(); ++i)
        dim += (*i)->getTypeSize();
      return dim;
    }
    /*
  case TYPE_INT:
    return sizeof(int);
  case TYPE_STRING:
    return sizeof(std::string);
  case TYPE_VOID:
    return 0;
    */
  case TYPE_ARRAY:
    return (arraySize * typeList.front()->getTypeSize());
  default:
    return sizeof(Operand);
  }
}


bool TypeDef::isPrimitive()
{
  switch (typeID)
  {
  case TYPE_INT:
  case TYPE_FLOAT:
  case TYPE_BOOL:
  case TYPE_STRING:
  case TYPE_VOID:
    return true;
  default:
    return false;
  }
}

bool TypeDef::isEqual(TypePtr otherType)
{
  if (isPrimitive())
  {
    return (typeID == otherType->typeID);
  }
  else if (typeID == TYPE_ARRAY)
  {
    if (otherType->typeID != TYPE_ARRAY)
      return false;
    //controlla che il sottotipo(arrayOf) sia identico
    TypePtr otherElemType = otherType->getArrayElemType();
    TypePtr thisElemType = this->getArrayElemType();

    return (thisElemType->isEqual(otherElemType));
  }
  else if (typeID == TYPE_CLASS)
  {
#ifdef EQUIV_STRUCT
    //equivlenza strutturale
    std::list<TypePtr>::iterator i, j;
    bool equal = true;

    //prima controlliamo che almeno abbiano lo stesso numero di sottotipi
    if (typeList.size() != otherType->typeList.size())
      return false;

    for (i = typeList.begin(), j = otherType->typeList.begin(); i != typeList.end(); ++i, ++j)
      equal |= (*i)->isEqual(*j);

    return equal;
#else
    return (this->getName() == otherType->getName());
#endif //EQUIV_STRUCT
  }
  else if (typeID == TYPE_PROD)
  {
    std::deque<TypePtr>::iterator i, j;
    bool equal = true;

    //prima controlliamo che almeno abbiano lo stesso numero di sottotipi
    if (typeList.size() != otherType->typeList.size())
      return false;

    for (i = typeList.begin(), j = otherType->typeList.begin(); i != typeList.end(); ++i, ++j)
      equal |= (*i)->isEqual(*j);

    return equal;
  }
  else return false;
}

bool TypeDef::sameParams(TypePtr otherType)
{
  std::deque<TypePtr>::iterator i, j, end;
  bool equal = true;

  //prima controlliamo che almeno abbiano lo stesso numero di sottotipi
  if (typeList.size() != otherType->typeList.size())
    return false;

  //ci fermiamo uno prima della fine (il valore di ritorno)
  end = typeList.end();
  --end;

  for (i = typeList.begin(), j = otherType->typeList.begin(); i != end; ++i, ++j)
    equal |= (*i)->isEqual(*j);

  return equal;
}

TypePtr TypeDef::resolveName(const std::string& name)
{
  std::deque<TypePtr>::iterator i;
  for (i = typeList.begin(); i != typeList.end(); ++i)
    if ((*i)->name == name)
      return (*i);

  //we have not found in the current scope, let's check it in global
  TypePtr globalScope = SymbolTable::GetSingleton().getGlobalNamespace()->type;
  for (i = globalScope->typeList.begin(); i != globalScope->typeList.end(); ++i)
    if ((*i)->name == name)
      return (*i);

  //raise error, there is not such type
  //non dovrebbe succedere, a lamentarsi doveva gia' essere il lexer
  std::string s = name + ": no such type";
  ErrorManager::GetSingleton().logAndRaise(s.c_str());  
  
  return TypeRoot::GetSingleton().getErr();
}

int TypeDef::getCost(TypePtr toType)
{
  if (this->typeID == TYPE_CLASS &&  toType->typeID == TYPE_CLASS)
  {
    //two objects
    if (this->getSymbol()->isMyAncestor(toType->getSymbol()))
      return costs[4][4];
  }
  else if ((this->typeID == TYPE_CLASS) && !(toType->typeID == TYPE_CLASS))
  {
    //provvisorio, niente costruttori!
    return INFINITE_THRES;
  }
  else if (!(this->typeID == TYPE_CLASS) && (toType->typeID == TYPE_CLASS))
  {
    //provvisorio, niente costruttori!
    return INFINITE_THRES;
  }
  else if (this->isPrimitive() && toType->isPrimitive())
  {
    return costs[this->typeID][toType->typeID];
  }
  return INFINITE_THRES;
}

bool TypeDef::isCompatible(TypePtr otherType)
{
  if (isEqual(otherType))
    return true;

  if (this->typeID == TYPE_OK && !otherType->isError())
    return true;

  if (otherType->typeID == TYPE_OK && !this->isError())
    return true;
  
  if (otherType->typeID == TYPE_ARRAY && this->typeID == TYPE_ARRAY)
    return this->getArrayElemType()->isCompatible(otherType->getArrayElemType()); 

  //equivalenza tra primitivi: int - float
  //equivalenza tra classi: upcast
  if (getCost(otherType) < AUTOMATIC_THRES)
    return true;

  return false;
}

bool TypeDef::isCastable(TypePtr otherType)
{
  if (isEqual(otherType))
    return true;
  
  if (otherType->typeID == TYPE_ARRAY && this->typeID == TYPE_ARRAY)
    return this->getArrayElemType()->isCompatible(otherType->getArrayElemType()); 

  //equivalenza tra primitivi: int - float
  //equivalenza tra classi: upcast
  if (getCost(otherType) < INFINITE_THRES)
    return true;

  return false;
}

//
// Type tree (class TypeRoot) implementation
////////////////////////////////////////////////////////

void TypeRoot::createStdTypes()
{ 
  //warning! do not modify order since it's relevant
  //(see TYPE_TYPE declaration order)
  addType(createType(TYPE_INT));
  addType(createType(TYPE_FLOAT));
  addType(createType(TYPE_STRING));
  addType(createType(TYPE_BOOL));
  addType(createType(TYPE_VOID));
  addType(createType(TYPE_ERR));
  addType(createType(TYPE_OK));

  //assert(costs[(int)TYPE_STRING][(int)TYPE_BOOL] == 9);
}

TypePtr TypeRoot::getStdType(TYPE_TYPE typeID)
{
  return (typeList[typeID]);
}


tag_t typeToTag(TypePtr type)
{
  tag_t tag;
  if (type->typeID == TYPE_CLASS /*|| type->typeID == TYPE_ARRAY*/)
  {
    tag = reinterpret_cast<tag_t>(type->getSymbol());
  }
  else
  {
    tag = type->typeID;
  }
  return tag;
}


//our Singleton TypeRoot
TypeRoot TypeRootSingleton;




