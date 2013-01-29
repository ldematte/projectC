
#ifndef VIRTUAL_MACHINE_H_INCLUDED_
#define VIRTUAL_MACHINE_H_INCLUDED_

#include "common.h"
#include "Singleton.h"
#include "../Instruction.h"
#include "ClassTable.h"

#include <stddef.h>


class VirtualMachine : public Singleton < VirtualMachine >
{
public:
  //registers
  Operand registers[REG_NUMBER];
  int eip;
  size_t instrStreamSize;
  
  Instruction* instrStream;
  Operand* Stack;
  bool debug;

public:

  void exec();
  Instruction getNextInstr();

  //stack manipulators
  void setStackValue(int idx, const Operand& val)
  {
    Stack[idx] = val;
  }

  Operand getStackValue(int idx)
  {
    return Stack[idx];
  }

  void setStackSize(size_t size);
  void setBaseAddress(Instruction* baseAddr) { instrStream = baseAddr; }
  
  void setEntryPoint()
  {
    eip = ClassTable::GetSingleton().getEntryPoint();
  }

  void reserveGlobal()
  {
    assert(Stack);
    int size = ClassTable::GetSingleton().getGlobalSize();
    registers[REG_ESP].intLiteral = size;
    registers[REG_EBP].intLiteral = size;
  }

  //Function Table
  FunctionEntry& getFunction(tag_t classTag, int idx)
  {
    ClassEntry& ce = ClassTable::GetSingleton().getClass(classTag);
    return ce.funcTable[idx];
  }

  FunctionEntry& getVirtualFunction(tag_t classTag, int idx)
  {
    int func_idx;
    ClassEntry& ce = ClassTable::GetSingleton().getClass(classTag);
    assert(idx < (int)ce.vtable.size());
    func_idx = ce.vtable[idx];
    return ce.funcTable[func_idx]; 
  }

  void dumpStack();
  void dumpCurrentFrame();

  void dumpDASM(const std::string& filename);

  
  VirtualMachine()
    : Stack(NULL), debug(false) {}

};


#endif //VIRTUAL_MACHINE_H_INCLUDED_


