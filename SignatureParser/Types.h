

#ifndef SIGNATURE_PARSER_H_INCLUDED_
#define SIGNATURE_PARSER_H_INCLUDED_

#include <string>


#include "../type.h"
#include "../NativeFunction.h"

class Function
{
public:
  Function(const std::string& name);
};

enum OPERATOR
{
  NEW_OP,
  DELETE_OP,       
  NEWA_OP,
  DELETEA_OP,
  PLUS_OP,
  MINUS_OP,
  TIMES_OP,
  DIV_OP,
  MOD_OP,
  BIT_XOR_OP,
  BIT_AND_OP,
  BIT_OR_OP,
  BIT_NOT_OP,
  NOT_OP,
  ASSIGN,
  EQ_OP,
  L_OP,
  G_OP,
  ADD_ASSIGN,
  SUB_ASSIGN,
  MUL_ASSIGN,
  DIV_ASSIGN,
  MOD_ASSIGN,
  XOR_ASSIGN,
  AND_ASSIGN,
  OR_ASSIGN,
  LSH_OP,
  RSH_OP,
  RIGHT_ASSIGN,
  LEFT_ASSIGN,
  NE_OP,
  LE_OP,
  GE_OP,
  AND_OP,
  OR_OP,
  INC_OP,
  DEC_OP,
  COMANOT_OP,
  ARROW_STAR_OP,
  ARROW_OP,
  LPAR_RPAR,
  LSQUARE_RSQUARE
};




class Operator : public Function
{
public:
  Operator(OPERATOR op);

};

class ClassType : public TypeDef
{
public:
  ClassType(const std::string& className)
  {
    name = className;
    symbol = NULL;
    type_id = TYPE_CLASS;
    isTemplate = false;
  }

  ClassType(const std::string& className, TypePtr subType)
  {
    name = className;
    symbol = NULL;
    type_id = TYPE_CLASS;
    isTemplate = false;
  }

  void addTemplateType(TypePtr tType)
  {
    isTemplate = true;
    templateList = tType;
  }

private:

  bool isTemplate;
  TypePtr templateList;
};


class FunctionDef
{
public:
  CallType callType;
  bool isVirtual;
  ACCESS_LEVEL accessLevel;
  bool isCtor;
  bool isDtor;
  TypePtr paramList;
};



#endif //SIGNATURE_PARSER_H_INCLUDED_

