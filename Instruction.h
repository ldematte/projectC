#ifndef INSTRUCTION_H_INCLUDED_
#define INSTRUCTION_H_INCLUDED_

#include "common.h"
#include "Operand.h"

#include <stdio.h>

enum OPCODE
{
  OP_ADD = 0,
  OP_SUB,
  OP_MUL,
  OP_DIV,
  OP_MOD,

  OP_INC,
  OP_DEC,
  
  OP_MOV,
  OP_SET,
  OP_PUSH,
  OP_POP,
  OP_TOP,
  OP_CLONE,
  
  OP_AND,
  OP_OR,
  OP_XOR,
  OP_NEG,
  OP_RSH,
  OP_LSH,

  OP_CALL,
  OP_RET,
  OP_VCALL,
  
  OP_FADD,
  OP_FSUB,
  OP_FMUL,
  OP_FDIV,
  OP_FSQRT,

  OP_I2F,
  OP_F2I,

  OP_JMP,
  OP_JNEG,
  OP_JZ,
  OP_JNZ,
  OP_JE,
  OP_JNE,
  OP_JLE,
  OP_JGE,
  OP_JG,
  OP_JL,

  OP_HALT,

  //string routines!!!

  OP_NEW,
  OP_NEWA,
  OP_LEA,
  OP_LEAA,
  OP_PADD,
  OP_PSUB,
  OP_PMOV,
  OP_STORE,
  OP_RELEASE,
  OP_DCAST, 
  OP_UCAST,

  OP_STR2I,
  OP_STR2F,
  OP_I2STR,
  OP_F2STR,

  OP_STRAT,
  OP_STRAPPEND,
  OP_NOP,
  OP_DEREF
};

struct Instruction
{
  OPCODE OpCode;

  //conviene avere 3 operandi anche per le istr che ne hanno 1
  Operand OperandList[3];

  OPCODE getOpCode() { return OpCode; }
  byte getOpCount();

  int jumpIndex();

  void dump(FILE* f);
  void dump() { dump(stderr); }

  void resolveVarOperands();
  void resolveTypeOperands();
};


#endif //INSTRUCTION_H_INCLUDED_

