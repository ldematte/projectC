#include "Instruction.h"
#include <stdio.h>

int Instruction::jumpIndex()
{
  switch (OpCode)
  {
  case OP_JZ:
  case OP_JNZ:
    return 1;

  case OP_JLE:
  case OP_JGE:
  case OP_JG:
  case OP_JL:
  case OP_JNE:
  case OP_JE:
    return 2;

  case OP_JMP:
    return 0;
    
  default:
    return -1;
  }
}

byte Instruction::getOpCount()
{
  switch (OpCode)
  {
  case OP_ADD:
  case OP_SUB:
  case OP_MUL:
  case OP_DIV:
  case OP_AND:
  case OP_OR:
  case OP_XOR:
  case OP_FADD:
  case OP_FSUB:
  case OP_FMUL:
  case OP_FDIV:
  case OP_RSH:
  case OP_LSH:
  case OP_NEWA:
  case OP_PADD:
  case OP_PSUB:
  case OP_STRAT:
  case OP_DCAST:
  case OP_UCAST:
    return 3;

  case OP_NEG:
  case OP_MOV:
  case OP_SET:
  case OP_FSQRT:
  case OP_I2F:
  case OP_F2I: 

  case OP_JNEG:
  case OP_JZ:
  case OP_JNZ:

  case OP_NEW: //var e tipo
  case OP_CALL:
  case OP_LEA:
  case OP_CLONE: 
  case OP_STORE:
  case OP_PMOV:
  case OP_STR2I:
  case OP_STR2F:
  case OP_I2STR:
  case OP_F2STR:
  case OP_STRAPPEND:
  case OP_DEREF:
    return 2;

  case OP_JLE:
  case OP_JGE:
  case OP_JG:
  case OP_JL:
  case OP_JNE:
  case OP_JE:
  case OP_LEAA:
    return 3;

  case OP_INC:
  case OP_DEC:
  case OP_PUSH:
  case OP_TOP:
  case OP_POP:
  case OP_JMP:
  case OP_RELEASE:
  case OP_RET:
  case OP_VCALL: //vtable index
    return 1;
    
  case OP_HALT:
  case OP_NOP:
    return 0;

  default:
    assert(false);
    return 0;
  }
}

char* opcodes[] = { "ADD", "SUB", "MUL", "DIV", "MOD", "INC", "DEC",
            "MOV", "SET", "PUSH", "POP", "TOP", "CLONE", 
            "AND", "OR", "XOR", "NEG", "RSH", "LSH", 
            "CALL", "RET", "VCALL",
            "FADD", "FSUB", "FMUL", "FDIV", "FSQRT", "I2F", "F2I",
            "JMP", "JNEG", "JZ", "JNZ", "JE", "JNE", "JLE", "JGE",
            "JG", "JL", "HALT", 
            "NEW", "NEWA", "LEA", "LEAA",
            "PADD", "PSUB", "PMOV", "STORE", "RELEASE", "DCAST", "UCAST", 
            "STR2I", "STR2F", "I2STR", "F2STR", "STRAT", "STRAPPEND", 
            "NOP", "DEREF"};

void Instruction::dump(FILE* f)
{
  fprintf(f, "%s", opcodes[(int)OpCode]);

  for (int i = 0; i < getOpCount(); ++i)
    OperandList[i].dump(f);
  fprintf(f, "\n");
}

void Instruction::resolveVarOperands()
{
  for (int i = 0; i < getOpCount(); ++i)
  {    
    if (OperandList[i].type_id == OP_TYPE_SYMBOL)
    {
      if (OperandList[i].symbol->isGlobal())
      {
        //e' una variabile globale
        OperandList[i].relStackIndex = OperandList[i].symbol->getAbsStackOffset();
        OperandList[i].type_id = OP_TYPE_ABS_STACK_INDEX;        
      }
      else
      {
        OperandList[i].relStackIndex = OperandList[i].symbol->getStackOffset();
        OperandList[i].type_id = OP_TYPE_REL_STACK_INDEX;
      }
    }
  }
}

void Instruction::resolveTypeOperands()
{
  for (int i = 0; i < getOpCount(); ++i)
  {
    if (OperandList[i].type_id == OP_TYPE_TYPE)
    {
      TypePtr type = OperandList[i].type;
      //if (!type->isPrimitive())
      if (type->typeID == TYPE_CLASS /*|| type->typeID == TYPE_ARRAY*/)
      {
        OperandList[i].objectTag = reinterpret_cast<tag_t>(type->getSymbol());
        OperandList[i].type_id = OP_TYPE_OBJECT_TAG;
      }
      else
      {
        OperandList[i].typeTag = type->typeID;
        OperandList[i].type_id = OP_TYPE_TYPE_TAG; 
      }
    }
  }
}

