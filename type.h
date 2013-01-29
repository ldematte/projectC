
#ifndef TYPE_H_INCLUDED_
#define TYPE_H_INCLUDED_

#include "common.h"
#include "constants.h"
#include "Singleton.h"
#include <deque>
#include <string>
#include <vector>

#include "symbolP.h"

enum ACCESS_LEVEL
{
  ACCESS_PUBLIC,
  ACCESS_PRIVATE,
  ACCESS_PROTECTED
};

//we need a type tree
enum TYPE_TYPE
{
  TYPE_INT = 0,
  TYPE_FLOAT,
  TYPE_STRING,
  TYPE_BOOL,
  TYPE_VOID,
  TYPE_ERR,  //type error during type checking
  TYPE_OK,   //this expression has no type, but it's well formed
  TYPE_ARRAY,
  TYPE_PROD, //product, for functions
  TYPE_CLASS,
  TYPE_CHAR,
  TYPE_WCHAR
};

class TypeDef;
typedef TypeDef* TypePtr;


//class that holds type definitions
//this is also the base of our type tree and
//permit nested type declaration
class TypeDef
{
public:

  TypeDef()
  {
    isConst = false;
    isReference = false;
    name = "_NO_NAME";
  }

  TYPE_TYPE typeID;

  //types used by this one
  std::deque<TypePtr> typeList;

  //add a "son" type (used for classes, arrays and functions)
  void addType(TypePtr type) { typeList.push_back(type); }
  void addParam(TypePtr type) { assert(typeID == TYPE_PROD); typeList.push_back(type); }

  //no. of types in typeList
  //can be more than one if we have an object or a product (function)
  size_t getListSize() { return typeList.size(); }
 
  TypePtr getRetType() { assert(typeID == TYPE_PROD); return typeList.back(); }
  
  std::vector<TypePtr> getParamTypes()
  {
    assert(typeID == TYPE_PROD);
    return std::vector<TypePtr>(typeList.begin(), prior(typeList.end()));
  }

  size_t getParamCount() { assert(typeID == TYPE_PROD); return (typeList.size() - 1); }
  TypePtr getArrayElemType() 
  { 
    assert(typeID == TYPE_ARRAY); 
    return typeList.front(); 
  }

  int getTypeSize();

  std::string getName();

  //type modifiers
  bool isConst;
  bool isReference;
  bool isPointer;

  int arraySize;

public:
  static TypePtr createArray(TypePtr arrayOf, int dim);
  
  //crea un array senza tipo, di dimesion pari a dim
  static TypePtr createEmptyArray(int dim);
  
  //no dimension: dynamic?
  static TypePtr createEmptyArray();
  
  static TypePtr createProduct();
  static TypePtr createClass(SymPtr symbol);
  static TypePtr createParameter(TypePtr type, bool byVal, bool isArray = false);
  static TypePtr createConst(TypePtr type);
  static TypePtr createPointer(TypePtr type);
  static TypePtr createError();
  
  //retrieve a typename in the form name or scope::name,
  //starting from the current scope. 
  //If the name does not exists, tryes the global namespae
  //if even this fails, returns a type error
  TypePtr resolveName(const std::string& name);

  //rules for merging two types
  bool isEqual(TypePtr otherType);
  bool sameParams(TypePtr otherType);
  bool isCompatible(TypePtr otherType);
  bool isCastable(TypePtr otherType);
  bool isError(); 
  bool isPrimitive();

  int getCost(TypePtr toType);

  SymPtr getSymbol() { assert(typeID == TYPE_CLASS); return symbol; }

protected:
  static TypePtr createType(TYPE_TYPE typeID);
  std::string name;

  SymPtr symbol;
};

//the root of our type tree
//must have global scope
class TypeRoot : public Singleton<TypeRoot>, public TypeDef
{
public:
  TypePtr getStdType(TYPE_TYPE typeID);
  TypePtr getErr()
  {
    return getStdType(TYPE_ERR);
  }
  void createStdTypes();

  //gets a typename in the form scope::name starting from the global
  //namespace and resolve it
  TypePtr resolveName(const std::string& name);
};
  
tag_t typeToTag(TypePtr type);


#endif //TYPE_H_INCLUDED_

