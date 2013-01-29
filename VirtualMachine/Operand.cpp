
#include "../Operand.h"
#include "VirtualMachine.h"
#include "RTStringTable.h"

int Operand::retrieveOperandAsInt() const
{
  Operand op = fullyResolveOperand();
  return op.intLiteral;
}

float Operand::retrieveOperandAsFloat() const
{
  Operand op = fullyResolveOperand();
  return op.floatLiteral;
}

//resolves only relative stack indices, registers
//and pointers
Operand Operand::retrieveOperandAsOperand() const
{
  switch (type_id)
  {
  /*
  case OP_TYPE_ABS_STACK_INDEX:
    return VirtualMachine::GetSingleton().getStackValue(absStackIndex);
  */

  case OP_TYPE_REL_STACK_INDEX:
    {
      Operand op;
      op.type_id = OP_TYPE_ABS_STACK_INDEX;

      //find an absolute stack index
      op.absStackIndex = VirtualMachine::GetSingleton().registers[REG_EBP].intLiteral + relStackIndex;
      return op;
    }  

  case OP_TYPE_REG:
    return VirtualMachine::GetSingleton().registers[regCode];

  case OP_TYPE_POINTER:
    return *((Operand*)pointer);

  default:
    return *this;
  }
}

//resolve completely an operand
Operand Operand::fullyResolveOperand() const
{
  switch (type_id)
  {  
  case OP_TYPE_ABS_STACK_INDEX:
    return VirtualMachine::GetSingleton().getStackValue(absStackIndex).fullyResolveOperand();

  case OP_TYPE_REL_STACK_INDEX:
    {
    //find an absolute stack index
    int absIndex = VirtualMachine::GetSingleton().registers[REG_EBP].intLiteral + relStackIndex;
    return VirtualMachine::GetSingleton().getStackValue(absIndex).fullyResolveOperand(); 
    }

  case OP_TYPE_REG:
    return VirtualMachine::GetSingleton().registers[regCode].fullyResolveOperand();

  case OP_TYPE_POINTER:
    return ((Operand*)pointer)->fullyResolveOperand();

  default:
    return *this;
  }
}

Operand Operand::resolveOperand() const
{
  switch (type_id)
  {  
  case OP_TYPE_ABS_STACK_INDEX:
    return VirtualMachine::GetSingleton().getStackValue(absStackIndex);

  case OP_TYPE_REL_STACK_INDEX:
    {
    //find an absolute stack index
    int absIndex = VirtualMachine::GetSingleton().registers[REG_EBP].intLiteral + relStackIndex;
    return VirtualMachine::GetSingleton().getStackValue(absIndex); 
    }

  case OP_TYPE_REG:
    return VirtualMachine::GetSingleton().registers[regCode];

  case OP_TYPE_POINTER:
    return *((Operand*)pointer);

  default:
    return *this;
  }
}


void Operand::storeOperand(int value)
{
  switch (type_id)
  {
  case OP_TYPE_ABS_STACK_INDEX:
    //a global
    VirtualMachine::GetSingleton().setStackValue(absStackIndex, Operand(value));
    break;
    
  case OP_TYPE_REL_STACK_INDEX:
    {
      //find an absolute stack index
      int absIndex = VirtualMachine::GetSingleton().registers[REG_EBP].intLiteral + relStackIndex;
      VirtualMachine::GetSingleton().setStackValue(absIndex, Operand(value));
      break;
    }

  case OP_TYPE_REG:
    if (VirtualMachine::GetSingleton().registers[regCode].type_id == OP_TYPE_POINTER)
    {
      ((Operand*)VirtualMachine::GetSingleton().registers[regCode].pointer)->intLiteral = value;
      ((Operand*)VirtualMachine::GetSingleton().registers[regCode].pointer)->type_id = OP_TYPE_INT;
    }
    else
    {
      VirtualMachine::GetSingleton().registers[regCode].intLiteral = value;
      VirtualMachine::GetSingleton().registers[regCode].type_id = OP_TYPE_INT;
    }
    break;

  case OP_TYPE_POINTER:
    ((Operand*)pointer)->intLiteral = value;
    ((Operand*)pointer)->type_id = OP_TYPE_INT;
    break;

  default:
    //error
    break;
  }
}

void Operand::storeOperand(float value)
{
  switch (type_id)
  {
  case OP_TYPE_ABS_STACK_INDEX:
    VirtualMachine::GetSingleton().setStackValue(absStackIndex, value);
    break;

  case OP_TYPE_REL_STACK_INDEX:
    {
      //find an absolute stack index
      int absIndex = VirtualMachine::GetSingleton().registers[REG_EBP].intLiteral + this->relStackIndex;
      VirtualMachine::GetSingleton().setStackValue(absIndex, value);
    }
    break;


  case OP_TYPE_REG:
    if (VirtualMachine::GetSingleton().registers[regCode].type_id == OP_TYPE_POINTER)
    {    
      ((Operand*)VirtualMachine::GetSingleton().registers[regCode].pointer)->floatLiteral = value;
      ((Operand*)VirtualMachine::GetSingleton().registers[regCode].pointer)->type_id = OP_TYPE_FLOAT;
    }
    else
    {
      VirtualMachine::GetSingleton().registers[regCode].floatLiteral = value;
      VirtualMachine::GetSingleton().registers[regCode].type_id = OP_TYPE_FLOAT;
    }
    break;

  case OP_TYPE_POINTER:
    ((Operand*)pointer)->floatLiteral = value;
    ((Operand*)pointer)->type_id = OP_TYPE_FLOAT;
    break;

  default:
    //error
    break;
  }
}

void Operand::storeOperand(Operand value)
{
  switch (type_id)
  {
  case OP_TYPE_ABS_STACK_INDEX:
    //a global
    VirtualMachine::GetSingleton().setStackValue(absStackIndex, value);
    break;
    
  case OP_TYPE_REL_STACK_INDEX:
    {
      //find an absolute stack index
      int absIndex = VirtualMachine::GetSingleton().registers[REG_EBP].intLiteral + relStackIndex;
      VirtualMachine::GetSingleton().setStackValue(absIndex, value);
      break;
    }

  case OP_TYPE_REG:
    if (VirtualMachine::GetSingleton().registers[regCode].type_id == OP_TYPE_POINTER)
      //TODO: ???
      VirtualMachine::GetSingleton().registers[regCode].storeOperand(value);
    else
      VirtualMachine::GetSingleton().registers[regCode] = value;
    break;

  case OP_TYPE_POINTER:
    *((Operand*)pointer) = value;
    break;

  default:
    //error
    break;
  }
}

char* Operand::retrieveOperandAsPointer() const
{
  switch (type_id)
  {
  case OP_TYPE_ABS_STACK_INDEX:
    assert(VirtualMachine::GetSingleton().getStackValue(absStackIndex).type_id == OP_TYPE_POINTER);
    return VirtualMachine::GetSingleton().getStackValue(absStackIndex).pointer;

  case OP_TYPE_REL_STACK_INDEX:
    {
      //find an absolute stack index
      int absIndex = VirtualMachine::GetSingleton().registers[REG_EBP].intLiteral + relStackIndex;
      assert(VirtualMachine::GetSingleton().getStackValue(absIndex).type_id == OP_TYPE_POINTER);
      return VirtualMachine::GetSingleton().getStackValue(absIndex).pointer;
    }

  case OP_TYPE_REG:
    assert((VirtualMachine::GetSingleton().registers[regCode].type_id == OP_TYPE_POINTER) ||
           (VirtualMachine::GetSingleton().registers[regCode].type_id == OP_TYPE_OBJECT) ||
           (VirtualMachine::GetSingleton().registers[regCode].type_id == OP_TYPE_ARRAY));
    return VirtualMachine::GetSingleton().registers[regCode].pointer;

  case OP_TYPE_POINTER:
    return pointer;

  default:
    return (char*)(&(this->intLiteral));
  }
}

void Operand::storeOperand(char* value)
{
  switch (type_id)
  {
  case OP_TYPE_ABS_STACK_INDEX:
    VirtualMachine::GetSingleton().setStackValue(absStackIndex, value);
    break;

  case OP_TYPE_REL_STACK_INDEX:
    {
      //find an absolute stack index
      int absIndex = VirtualMachine::GetSingleton().registers[REG_EBP].intLiteral + this->relStackIndex;
      VirtualMachine::GetSingleton().setStackValue(absIndex, value);
    }
    break;

  case OP_TYPE_REG:
    VirtualMachine::GetSingleton().registers[regCode].pointer = value;
    VirtualMachine::GetSingleton().registers[regCode].type_id = OP_TYPE_POINTER;
    break;

  case OP_TYPE_POINTER:
    ((Operand*)pointer)->pointer = value;
    ((Operand*)pointer)->type_id = OP_TYPE_POINTER;
    break;

  default:
    //raise error
    break;
  }
}

void Operand::storeOperand(Object* value)
{
switch (type_id)
  {
  case OP_TYPE_ABS_STACK_INDEX:
    VirtualMachine::GetSingleton().setStackValue(absStackIndex, value);
    break;

  case OP_TYPE_REL_STACK_INDEX:
    {
      //find an absolute stack index
      int absIndex = VirtualMachine::GetSingleton().registers[REG_EBP].intLiteral + this->relStackIndex;
      VirtualMachine::GetSingleton().setStackValue(absIndex, value);
    }
    break;

  case OP_TYPE_REG:
    if (VirtualMachine::GetSingleton().registers[regCode].type_id == OP_TYPE_POINTER)
    {    
      ((Operand*)VirtualMachine::GetSingleton().registers[regCode].pointer)->objAddr = value;
      ((Operand*)VirtualMachine::GetSingleton().registers[regCode].pointer)->type_id = OP_TYPE_OBJECT;
    }
    else
    {
      VirtualMachine::GetSingleton().registers[regCode].objAddr = value;
      VirtualMachine::GetSingleton().registers[regCode].type_id = OP_TYPE_OBJECT;
    }
    break;

  case OP_TYPE_POINTER:
    ((Operand*)pointer)->objAddr = value;
    ((Operand*)pointer)->type_id = OP_TYPE_OBJECT;
    break;

  default:
    //raise error
    break;
  }
}

char* Operand::retrievePointerToOperand() const
{
  switch (type_id)
  {
  case OP_TYPE_ABS_STACK_INDEX:
    return ((char*)(&(VirtualMachine::GetSingleton().Stack[absStackIndex])));

  case OP_TYPE_REL_STACK_INDEX:
    {
      //find an absolute stack index
      int absIndex = VirtualMachine::GetSingleton().registers[REG_EBP].intLiteral + relStackIndex;
      return ((char*)(&(VirtualMachine::GetSingleton().Stack[absIndex])));
    }

  case OP_TYPE_REG:
    //raise error
    assert(0);
    return VirtualMachine::GetSingleton().registers[regCode].pointer;

  case OP_TYPE_POINTER:
    return pointer;

  case OP_TYPE_OBJECT:
  case OP_TYPE_ARRAY:
    {
      Operand  op = retrieveOperandAsOperand();
      char* objPtr = op.objAddr->getData();
      return (objPtr);
    }
    break;

  case OP_TYPE_STRING:
    {
      std::string& s = RTStringTable::GetSingleton().getString(this->stringLiteralIdx);
      return ((char*)&s);
    }

  default:
    return ((char*)this);
  }
}



void Operand::storeStringOperand(int index)
{
  switch (type_id)
  {
  case OP_TYPE_ABS_STACK_INDEX:
    {
      Operand op;
      op.type_id = OP_TYPE_STRING;
      op.stringLiteralIdx = index;
    
      //a global
      VirtualMachine::GetSingleton().setStackValue(absStackIndex, op);
    }
    break;
    
  case OP_TYPE_REL_STACK_INDEX:
    {
      Operand op;
      op.type_id = OP_TYPE_STRING;
      op.stringLiteralIdx = index;

      //find an absolute stack index
      int absIndex = VirtualMachine::GetSingleton().registers[REG_EBP].intLiteral + relStackIndex;
      VirtualMachine::GetSingleton().setStackValue(absIndex, op);
      break;
    }

  case OP_TYPE_REG:
    if (VirtualMachine::GetSingleton().registers[regCode].type_id == OP_TYPE_POINTER)
    {
      ((Operand*)VirtualMachine::GetSingleton().registers[regCode].pointer)->stringLiteralIdx = index;
      ((Operand*)VirtualMachine::GetSingleton().registers[regCode].pointer)->type_id = OP_TYPE_STRING;
    }
    else
    {
      VirtualMachine::GetSingleton().registers[regCode].stringLiteralIdx = index;
      VirtualMachine::GetSingleton().registers[regCode].type_id = OP_TYPE_STRING;
    }
    break;

  case OP_TYPE_POINTER:
    ((Operand*)pointer)->stringLiteralIdx = index;
    ((Operand*)pointer)->type_id = OP_TYPE_STRING;
    break;

  default:
    //error
    break;
  }
}


