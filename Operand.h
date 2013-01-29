

#ifndef _OPERAND_H_INCLUDED_
#define _OPERAND_H_INCLUDED_

#include <iostream>
#include <stdio.h>
#include <string>

#include "common.h"

//cambia se siamo nel compilatore o nella vm
#ifdef COMPILER_ENVIRONMENT
  #define STRING_TABLE StringTable::GetSingleton()
#else
  #include "VirtualMachine/RTStringTable.h"
  #define STRING_TABLE RTStringTable::GetSingleton()
#endif //COMPILER_ENVIRONMENT

#include "compiler/symbol.h"
#include "compiler/StringTable.h"

#include <Object.h>

#define REG_EBP 0
#define REG_EIP 1
#define REG_ESP 2
#define REG_RET 3
#define REG_A   4
#define REG_B   5
#define REG_C   6
#define REG_D   7
#define REG_E   8

#define REG_NUMBER 9

enum OPTYPE
{
  //immediate values
  OP_TYPE_INT,
  OP_TYPE_FLOAT,
  OP_TYPE_STRING,

  OP_TYPE_ABS_STACK_INDEX,
  OP_TYPE_REL_STACK_INDEX,

  OP_TYPE_OBJECT,  //Object* 
  OP_TYPE_POINTER, //char*
  OP_TYPE_ARRAY,

  OP_TYPE_INSTR_INDEX,
  OP_TYPE_FUNC,
  OP_TYPE_REG,

  OP_TYPE_OBJECT_TAG,
  OP_TYPE_TYPE_TAG,

  //not fully resolved reference to the symbol table
  //valid only in I-Code
  OP_TYPE_SYMBOL,
  OP_TYPE_TYPE
};



class Operand
{
public:
  OPTYPE type_id;

  union
  {
    int intLiteral;
    float floatLiteral;
    int stringLiteralIdx;

    //e metterere i SymPtr nel codice intremedio??
    int absStackIndex;
    int relStackIndex;
    int instrIndex;
    int funcIndex;

    Object* objAddr;
    char* pointer;

    int regCode;
    tag_t objectTag;
    TYPE_TYPE typeTag;

    //this is valid ONLY in I-Code
    SymPtr symbol;
    TypePtr type;
  };

private:

  const char* regCodeToName(int regCode) const
  {
    switch (regCode)
    {
    case REG_EBP:
      return "_EBP";
    case REG_EIP:
      return "_EIP";
    case REG_ESP:
      return "_ESP";
    case REG_RET:
      return "_RET";
    case REG_A:
      return "_A";
    case REG_B:
      return "_B";
    case REG_C:
      return "_C";
    case REG_D:
      return "_D";
    }
    return "_UNKN";
  }

public:
  Operand() { }

  Operand(int i)
  {
    type_id = OP_TYPE_INT;
    intLiteral = i;
  }

  Operand(Object* o)
  {
    type_id = OP_TYPE_OBJECT;
    objAddr = o;
  }

  Operand(float f)
  {
    type_id = OP_TYPE_FLOAT;
    floatLiteral = f;
  }

  Operand(const std::string& s)
  {
    int idx = StringTable::GetSingleton().insert(s);
    stringLiteralIdx = idx;
    type_id = OP_TYPE_STRING;
  }

  Operand(SymPtr sym)
  {
    symbol = sym;
    type_id = OP_TYPE_SYMBOL;
  }

  Operand(char* p)
  {
    type_id = OP_TYPE_POINTER;
    pointer = p;
  }

  int retrieveOperandAsInt() const;
  float retrieveOperandAsFloat() const;
  Operand retrieveOperandAsOperand() const;
  char* retrieveOperandAsPointer() const;
  char* retrievePointerToOperand() const;

  Operand resolveOperand() const;
  Operand fullyResolveOperand() const;

  void storeOperand(int value);
  void storeOperand(float value);
  void storeOperand(Operand value);
  void storeOperand(char* value);
  void storeOperand(Object* value);
  void storeStringOperand(int index);

  bool isEqual(const Operand& other)
  {
    return (type_id == other.type_id && absStackIndex == other.absStackIndex);
  }

  void dump()
  {
    dump(stderr);
  }

  void dump(FILE* f)
  {
    switch(type_id)
    {
    case OP_TYPE_STRING:
      fprintf(f, " \"%s\" ", STRING_TABLE.getString(stringLiteralIdx).c_str());
      break;

    case OP_TYPE_SYMBOL:
      fprintf(f, " %s ", symbol->name.c_str());
      break;

    case OP_TYPE_FUNC:
      fprintf(f, " func #%d ", funcIndex);
      break;

    case OP_TYPE_REG:
      fprintf(f, " REG%s ", regCodeToName(regCode));
      //VirtualMachine::GetSingleton().registers[intLiteral].dump(f);
      break;

    case OP_TYPE_FLOAT:
      fprintf(f, " %f ", floatLiteral);
      break;

    case OP_TYPE_OBJECT_TAG:
      fprintf(f, " object no:%d", (int)objectTag);
      break;

    case OP_TYPE_TYPE_TAG:
      fprintf(f, " type no:%d", typeTag);
      break;

    case OP_TYPE_OBJECT:
      fprintf(f, " 0x%x ", (ptrdiff_t)objAddr);
      if (objAddr && (objAddr != (Object*)BAD_PTR) )
      {
        fprintf(f, " (data: 0x%x), ", (ptrdiff_t)objAddr->data);
        fprintf(f, " (is: %d,", (int)(objAddr->actualID));
        fprintf(f, " was: %d)", (int)(objAddr->originalID));
        fprintf(f, " COUNT: %d", objAddr->data->refCount);
      }
      break;
    case OP_TYPE_ARRAY:
      fprintf(f, " 0x%x ", (ptrdiff_t)objAddr);
      if (objAddr && (objAddr != (Object*)BAD_PTR) )
      {
        fprintf(f, " ARRAY (data: 0x%x)", (ptrdiff_t)objAddr->getData());
      }
      break;


    case OP_TYPE_POINTER:
      fprintf(f, " 0x%x ", (ptrdiff_t)pointer);
      break;

    default:
      fprintf(f, " %d ", intLiteral);
      break; 
    }
  }
};



#endif //_OPERAND_H_INCLUDED_


