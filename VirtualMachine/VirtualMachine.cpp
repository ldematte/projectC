#include "VirtualMachine.h"
#include "ClassTable.h"
#include "RTStringTable.h"
#include "RTSysExportTable.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include <string>
#include <iostream>
#include <sstream>

#include "../constants.h"
#include "../NativeFunction.h"
#include "NativeCall.h"


Operand addressOf(Operand& op)
{
  Operand ret;
  ret.type_id = OP_TYPE_POINTER;

  switch (op.type_id)
  {
  case OP_TYPE_OBJECT:
    {          
      //the refCount size
      char* objPtr = ((char*)op.objAddr->data) + OBJDATA_HEADER_SIZE;
      ret.pointer = objPtr;
    }
    break;
  
  //case OP_TYPE_STRING:
  case OP_TYPE_ARRAY:
    {
      char* objPtr = op.objAddr->getData();
      ret.pointer = objPtr;
    }
    break;

  case OP_TYPE_POINTER:
    {
      //che tipo e' quello puntato?
      Operand deref = *((Operand*)(op.pointer));
      return addressOf(deref);
    }
    break;
  
  default:
    {
      //int, float, bool
      char* address =  op.retrievePointerToOperand();
      ret.pointer = address;
    }
    break;
  }

  return ret;
}



Instruction VirtualMachine::getNextInstr()
{
  return (instrStream[eip]);
}

void VirtualMachine::dumpDASM(const std::string& filename)
{
  FILE* f = fopen(filename.c_str(), "w");

  for (size_t i = 0; i < instrStreamSize; ++i)
  {
    fprintf(f, "%i:\t", i);
    instrStream[i].dump(f);
    fflush(f);
  }

  fclose(f);
}

void VirtualMachine::exec()
{

  for (;;)
  {
    int oldEip = eip;

    //debug = true;

    //fetch
    Instruction current = getNextInstr();
    if (debug)
    {
      std::cout << "Executing: " ;
      fprintf(stderr, "%i : ", eip);
      current.dump();

    }

    /*
    if (eip == 17)
    {
      debug = true;
    }
    */
    
    //decode
    switch (current.OpCode)
    {
    case OP_ADD:
      {
        //retrieve operands
        int op1 = current.OperandList[1].retrieveOperandAsInt();
        int op2 = current.OperandList[2].retrieveOperandAsInt();
        //execute
        current.OperandList[0].storeOperand(op1 + op2);
      }
      break;

    case OP_SUB:
      {
        //retrieve operands
        int op1 = current.OperandList[1].retrieveOperandAsInt();
        int op2 = current.OperandList[2].retrieveOperandAsInt();
        //execute
        current.OperandList[0].storeOperand(op1 - op2);
      }
      break;

    case OP_MUL:
      {
        //retrieve operands
        int op1 = current.OperandList[1].retrieveOperandAsInt();
        int op2 = current.OperandList[2].retrieveOperandAsInt();
        //execute
        current.OperandList[0].storeOperand(op1 * op2);
      }
      break;

    case OP_DIV:
      {
        //retrieve operands
        int op1 = current.OperandList[1].retrieveOperandAsInt();
        int op2 = current.OperandList[2].retrieveOperandAsInt();
        //execute
        current.OperandList[0].storeOperand(op1 / op2);
      }
      break;

    case OP_MOD:
      {
        //retrieve operands
        int op1 = current.OperandList[1].retrieveOperandAsInt();
        int op2 = current.OperandList[2].retrieveOperandAsInt();
        //execute
        current.OperandList[0].storeOperand(op1 % op2);
      }
      break;

    case OP_INC:
      {
        int op1 = current.OperandList[0].retrieveOperandAsInt();
        //execute
        current.OperandList[0].storeOperand(op1 + 1);
      }
      break;

    case OP_DEC:
      {
        int op1 = current.OperandList[0].retrieveOperandAsInt();
        //execute
        current.OperandList[0].storeOperand(op1 - 1);
      }
      break;

    case OP_MOV:
      {
        current.OperandList[0].storeOperand(current.OperandList[1].fullyResolveOperand());
      }
      break;

    case OP_SET:
      {
        assert(current.OperandList[0].type_id == OP_TYPE_REG || 
               current.OperandList[0].type_id == OP_TYPE_REL_STACK_INDEX ||
               current.OperandList[0].type_id == OP_TYPE_ABS_STACK_INDEX);
        switch (current.OperandList[0].type_id)
        {
        case OP_TYPE_REG:
          registers[current.OperandList[0].regCode] = current.OperandList[1].fullyResolveOperand();
          break;
        case OP_TYPE_ABS_STACK_INDEX:
          setStackValue(current.OperandList[0].absStackIndex, current.OperandList[1].fullyResolveOperand());
          break;

        case OP_TYPE_REL_STACK_INDEX:
          {
            int absIndex = registers[REG_EBP].intLiteral + current.OperandList[0].relStackIndex;
            setStackValue(absIndex, current.OperandList[1].fullyResolveOperand());
            break;
          }
        }
      }
      break;

      //stack
    case OP_PUSH:
      {
        Operand op = current.OperandList[0].resolveOperand();
        //Operand op = current.OperandList[0].fullyResolveOperand();
        if ((op.type_id == OP_TYPE_OBJECT || op.type_id == OP_TYPE_ARRAY) && op.objAddr != (Object*)BAD_PTR)
          op.objAddr->addRef();
        setStackValue(registers[REG_ESP].intLiteral, op);
        ++registers[REG_ESP].intLiteral;
      }
      break;

    case OP_POP:
      {
        assert(registers[REG_ESP].intLiteral > 0);

        if (current.OperandList[0].type_id == OP_TYPE_REG)
        {
          registers[current.OperandList[0].regCode] = getStackValue(registers[REG_ESP].intLiteral - 1);
        }
        else
        { 
          current.OperandList[0].storeOperand(getStackValue(registers[REG_ESP].intLiteral - 1));
        }
        --registers[REG_ESP].intLiteral;
      }
      break;

    case OP_TOP:
      {
        current.OperandList[0].storeOperand(getStackValue(registers[REG_ESP].intLiteral - 1));
      }
      break;

    case OP_CLONE:
      {
        Operand src = current.OperandList[1].fullyResolveOperand();

        if (src.objAddr == (Object*)BAD_PTR)
          break;

        assert(src.type_id == OP_TYPE_OBJECT ||
               src.type_id == OP_TYPE_STRING /* || src.type_id == OP_TYPE_ARRAY*/);
        switch(src.type_id)
        {
        case OP_TYPE_OBJECT:
          {
            Object* oldObj = src.objAddr;

            //allocate 
            Object* newObj = new Object;

            //initialize
            newObj->data = oldObj->data;
            newObj->actualID = oldObj->actualID;
            newObj->originalID = oldObj->originalID;
            //newObj->addRef(); //ci pensa la PUSH

            //FIX: non store, ma come in SET!! SPECIE
            //se dst e' un reg!!
            if (current.OperandList[0].type_id == OP_TYPE_REG)
            {
              Operand obj;
              obj.type_id = OP_TYPE_OBJECT;
              obj.objAddr = newObj;

              registers[current.OperandList[0].regCode] =  obj;
            }
            else
              current.OperandList[0].storeOperand(newObj);
          }
          break;

        case OP_TYPE_STRING:
          //TODO
          break;

        /*
        case OP_TYPE_ARRAY:
          //TODO
          break;
        */
        }
      }
      break;

    case OP_AND:
      {
        int a = current.OperandList[1].retrieveOperandAsInt();
        int b = current.OperandList[2].retrieveOperandAsInt();

        current.OperandList[0].storeOperand(a & b);        
      }
      break;

    case OP_OR:
      {
        int a = current.OperandList[1].retrieveOperandAsInt();
        int b = current.OperandList[2].retrieveOperandAsInt();

        current.OperandList[0].storeOperand(a | b);        
      }
      break;

    case OP_XOR:
      {
        int a = current.OperandList[1].retrieveOperandAsInt();
        int b = current.OperandList[2].retrieveOperandAsInt();

        current.OperandList[0].storeOperand(a ^ b);        
      }
      break;

    case OP_NEG:
      {
        int a = current.OperandList[1].retrieveOperandAsInt();

        current.OperandList[0].storeOperand(~a);        
      }
      break;

    case OP_RSH:
      {
        int a = current.OperandList[1].retrieveOperandAsInt();
        int b = current.OperandList[2].retrieveOperandAsInt();

        current.OperandList[0].storeOperand(a >> b);        
      }

    case OP_LSH:
      {
        int a = current.OperandList[1].retrieveOperandAsInt();
        int b = current.OperandList[2].retrieveOperandAsInt();

        current.OperandList[0].storeOperand(a << b);        
      }
      break;

    //function call
    case OP_CALL:
      {
        //this is the function index (or pointer?)
        assert(current.OperandList[1].type_id == OP_TYPE_FUNC);
        assert(current.OperandList[0].type_id == OP_TYPE_OBJECT_TAG);
        FunctionEntry f = getFunction(current.OperandList[0].objectTag , current.OperandList[1].funcIndex);

        //save return address
        /*
        setStackValue(registers[REG_ESP].intLiteral, Operand(eip));
        ++registers[REG_ESP].intLiteral;
        */

        if (f.isNative)
        {
          NativeFunction& ne = RTSysExportTable::GetSingleton().get(f);

          DWORD dwRet = 0;
          //necessaria qui
          std::string returnString("__FRA__");
            

          switch(ne.callType)
          {
          case CALL_CDECL:
            {
              //DWORD saveEIP = GetEIP();

              //build up arg list
              //TODO: strings
              int paramSize = f.paramCount * NAKED_WORD_SIZE;
              if (ne.returnType == OP_TYPE_STRING)
              {
                //|| ne.returnType == OP_TYPE_ARRAY || ne.returnType == OP_TYPE_OBJECT)???
                paramSize = paramSize + NAKED_WORD_SIZE;
              }

              char* args = new char[paramSize];
              char* p = args;

              if (ne.returnType == OP_TYPE_STRING)
              {
                //if string return type
                std::string* strPointer = &returnString;
                memcpy(p, &strPointer, NAKED_WORD_SIZE);
                p += NAKED_WORD_SIZE;
              }

              int begin = registers[REG_ESP].intLiteral + CALL_PARAMS_OFFSET;
              int end = begin - f.paramCount;

              //TODO: check order!!
              for (int i = begin; i > end; --i)
              {
                if (Stack[i].type_id == OP_TYPE_STRING)
                {
                  std::string& s = RTStringTable::GetSingleton().getString(Stack[i].stringLiteralIdx);
                  char* pointer = (char*)&s;
                  memcpy(p, &(pointer), NAKED_WORD_SIZE);
                }
                else
                  memcpy(p, &(Stack[i].instrIndex), NAKED_WORD_SIZE);
                
                p += NAKED_WORD_SIZE;
              }

              dwRet = Call_cdecl(args, paramSize, (DWORD)ne.proc);
              delete[] args;

              //the RA is on the top of the stack
              /*
              Operand retAddr;
              retAddr.type_id = OP_TYPE_ABS_STACK_INDEX;
              retAddr.absStackIndex = registers[REG_ESP].intLiteral - 1;

              //next instruction
              eip = retAddr.retrieveOperandAsInt() + 1;
              */

              //dumpStack();

               //release this
              int idx = registers[REG_ESP].intLiteral - 1; //la posizione di this

              assert(Stack[idx].type_id == OP_TYPE_OBJECT);

              if (!(Stack[idx].objAddr == (Object*)BAD_PTR))
              {
                Stack[idx].objAddr->release();
                delete Stack[idx].objAddr;
                Stack[idx].objAddr = NULL;
              }

              //clear up n operands (params put on the stack of the VM)
              registers[REG_ESP].intLiteral -= (f.paramCount + 1); //this        
              
              //release other objects (TODO)
              
            }
            break;

          default:
            assert(0);
            break;

          }

          Operand ret;
          ret.type_id = (OPTYPE)ne.returnType;
          switch(ret.type_id)
          {
          case OP_TYPE_STRING:
            {
              int strIndex = RTStringTable::GetSingleton().getNewEmptyString();
              RTStringTable::GetSingleton().setString(strIndex, returnString);
              ret.stringLiteralIdx = strIndex;
            }
            break;
            
          case OP_TYPE_FLOAT:
            {
              //evitiamo 2 cast inutili
              ret.intLiteral = GetST0();
            }
            break;

          default:
            ret.intLiteral = dwRet;
            break;
          }

          registers[REG_RET] = ret;
        }
        else
        {
          //save return address
          setStackValue(registers[REG_ESP].intLiteral, Operand(eip));
          ++registers[REG_ESP].intLiteral;

          //prolog
          /*
          //now made by compiler
          setStackValue(registers[REG_ESP].intLiteral, Operand(registers[REG_EBP].intLiteral));
          registers[REG_EBP].intLiteral = registers[REG_ESP].intLiteral;
          //+ 1 for the saved registers[REG_EBP].intLiteral
          registers[REG_ESP].intLiteral = registers[REG_ESP].intLiteral + f.localCount + 1;
          */
          eip = f.startAddress;
        }
      }
      break;

    case OP_RET:
      {
        //the RA is on the top of the stack
        Operand retAddr;
        retAddr.type_id = OP_TYPE_ABS_STACK_INDEX;
        retAddr.absStackIndex = registers[REG_ESP].intLiteral - 1;

        //next instruction
        eip = retAddr.retrieveOperandAsInt() + 1;

        //clear up n operands (params put on the stack)
        registers[REG_ESP].intLiteral -= current.OperandList[0].retrieveOperandAsInt();
      }
      break;

    case OP_VCALL:
      {        
        Operand This;
        Operand thisPos;

        //nel caso di VCALL, this e' il primo parametro
        //ricorda pero' che dobbiamo ancora fare
        //il prologo!
        //int paramCount = current.OperandList[1].intLiteral;
        thisPos.type_id = OP_TYPE_ABS_STACK_INDEX;
        thisPos.absStackIndex = registers[REG_ESP].intLiteral - 1; //esp e' uno sopra l'ultimo

        This = thisPos.resolveOperand();   

        //dumpStack();

        tag_t classTag = This.objAddr->originalID;
        FunctionEntry f = getVirtualFunction(classTag, current.OperandList[0].funcIndex);

        //save return address
        setStackValue(registers[REG_ESP].intLiteral, Operand(eip));
        ++registers[REG_ESP].intLiteral;

        eip = f.startAddress;
      }
      break;

    //floating point
    case OP_FADD:
      {
        //retrieve operands
        float op1 = current.OperandList[1].retrieveOperandAsFloat();
        float op2 = current.OperandList[2].retrieveOperandAsFloat();
        //execute
        current.OperandList[0].storeOperand(op1 + op2);
      }
      break;

    case OP_FSUB:
      {
        //retrieve operands
        float op1 = current.OperandList[1].retrieveOperandAsFloat();
        float op2 = current.OperandList[2].retrieveOperandAsFloat();
        //execute
        current.OperandList[0].storeOperand(op1 - op2);
      }
      break;

    case OP_FMUL:
      {
        //retrieve operands
        float op1 = current.OperandList[1].retrieveOperandAsFloat();
        float op2 = current.OperandList[2].retrieveOperandAsFloat();
        //execute
        current.OperandList[0].storeOperand(op1 * op2);
      }
      break;

    case OP_FDIV:
      //TODO: raise exception
      {
        //retrieve operands
        float op1 = current.OperandList[1].retrieveOperandAsFloat();
        float op2 = current.OperandList[2].retrieveOperandAsFloat();
        //execute
        current.OperandList[0].storeOperand(op1 / op2);
      }
      break;

    case OP_FSQRT:
      {
        //TODO: raise exception
        //retrieve operands
        float op1 = current.OperandList[1].retrieveOperandAsFloat();
        //execute
        current.OperandList[0].storeOperand(sqrtf(op1));
      }
      break;

    case OP_I2F:
      {
        int op1 = current.OperandList[1].retrieveOperandAsInt();
        //execute
        float f = (float)op1;
        current.OperandList[0].storeOperand(f);
      }      
      break;

    case OP_F2I:
      {
        float op1 = current.OperandList[1].retrieveOperandAsFloat();
        //execute
        int i = (int)op1;
        current.OperandList[0].storeOperand(i);
      }   
      break;

      //jump
    case OP_JMP:
      {
        assert(current.OperandList[0].type_id == OP_TYPE_INSTR_INDEX);
        eip = current.OperandList[0].instrIndex;
      }
      break;

    case OP_JNEG:
      {
        assert(current.OperandList[1].type_id == OP_TYPE_INSTR_INDEX);

        Operand op0 = current.OperandList[0].fullyResolveOperand();

        switch (op0.type_id)
        {
        case OP_TYPE_INT:
        case OP_TYPE_INSTR_INDEX:
          if (op0.intLiteral < 0)
            eip = current.OperandList[1].instrIndex;
          else
            ++eip;
          break;

        case OP_TYPE_FLOAT:
          if (op0.floatLiteral < 0)
            eip = current.OperandList[1].instrIndex;
          else
            ++eip;
          break;
        }
      }
      break;

    case OP_JZ:
      {
        assert(current.OperandList[1].type_id == OP_TYPE_INSTR_INDEX);

        Operand op0 = current.OperandList[0].fullyResolveOperand();

        switch (op0.type_id)
        {
        case OP_TYPE_INT:
        case OP_TYPE_INSTR_INDEX:
          if (op0.intLiteral == 0)
            eip = current.OperandList[1].instrIndex;
          else
            ++eip;
          break;

        case OP_TYPE_FLOAT:
          if (op0.floatLiteral == 0.0f)
            eip = current.OperandList[1].instrIndex;
          else
            ++eip;
          break;

        case OP_TYPE_OBJECT:
        case OP_TYPE_ARRAY:
        case OP_TYPE_STRING:
          //TODO
          break;
        }
      }
      break;

    case OP_JNZ:
      {
        assert(current.OperandList[1].type_id == OP_TYPE_INSTR_INDEX);

        Operand op0 = current.OperandList[0].fullyResolveOperand();
   //     if (op0.type_id == OP_TYPE_POINTER)
   //       op0.resolveOperand();

        switch (op0.type_id)
        {
        case OP_TYPE_INT:
        case OP_TYPE_INSTR_INDEX:
          if (op0.intLiteral != 0)
            eip = current.OperandList[1].instrIndex;
          else
            ++eip;
          break;

        case OP_TYPE_FLOAT:
          if (op0.floatLiteral != 0.0f)
            eip = current.OperandList[1].instrIndex;
          else
            ++eip;
          break;


        case OP_TYPE_OBJECT:
        case OP_TYPE_ARRAY:
        case OP_TYPE_STRING:
          //TODO
          assert(0);
          break;
        }
      }
      break;

    case OP_JE:
      {
        assert(current.OperandList[2].type_id == OP_TYPE_INSTR_INDEX);

        Operand op0 = current.OperandList[0].fullyResolveOperand();
        Operand op1 = current.OperandList[1].fullyResolveOperand();

        switch (op0.type_id)
        {
        case OP_TYPE_INT:
        case OP_TYPE_INSTR_INDEX:
          if (op0.intLiteral == op1.intLiteral)
            eip = current.OperandList[2].instrIndex;
          else
            ++eip;
          break;

        case OP_TYPE_FLOAT:
          if (op0.floatLiteral == op1.floatLiteral)
            eip = current.OperandList[2].instrIndex;
          else
            ++eip;
          break;

        case OP_TYPE_OBJECT:
        case OP_TYPE_ARRAY:
        case OP_TYPE_STRING:
          //TODO
          break;
        }
      }
      break;

    case OP_JNE:
      {
        assert(current.OperandList[2].type_id == OP_TYPE_INSTR_INDEX);

        Operand op0 = current.OperandList[0].fullyResolveOperand();
        Operand op1 = current.OperandList[1].fullyResolveOperand();

        switch (op0.type_id)
        {
        case OP_TYPE_INT:
        case OP_TYPE_INSTR_INDEX:
          if (op0.intLiteral != op1.intLiteral)
            eip = current.OperandList[2].instrIndex;
          else
            ++eip;
          break;

        case OP_TYPE_FLOAT:
          if (op0.floatLiteral != op1.floatLiteral)
            eip = current.OperandList[2].instrIndex;
          else
            ++eip;
          break;

        case OP_TYPE_OBJECT:
        case OP_TYPE_ARRAY:
        case OP_TYPE_STRING:
          //TODO
          break;
        }
      }
      break;

    case OP_JLE:
      {
        assert(current.OperandList[2].type_id == OP_TYPE_INSTR_INDEX);

        Operand op0 = current.OperandList[0].fullyResolveOperand();
        Operand op1 = current.OperandList[1].fullyResolveOperand();

        switch (op0.type_id)
        {
        case OP_TYPE_INT:
        case OP_TYPE_INSTR_INDEX:
          if (op0.intLiteral <= op1.intLiteral)
            eip = current.OperandList[2].instrIndex;
          else
            ++eip;
          break;

        case OP_TYPE_FLOAT:
          if (op0.floatLiteral <= op1.floatLiteral)
            eip = current.OperandList[2].instrIndex;
          else
            ++eip;
          break;
        }
      }
      break;

    case OP_JGE:
      {
        assert(current.OperandList[2].type_id == OP_TYPE_INSTR_INDEX);

        Operand op0 = current.OperandList[0].fullyResolveOperand();
        Operand op1 = current.OperandList[1].fullyResolveOperand();

        switch (op0.type_id)
        {
        case OP_TYPE_INT:
        case OP_TYPE_INSTR_INDEX:
          if (op0.intLiteral >= op1.intLiteral)
            eip = current.OperandList[2].instrIndex;
          else
            ++eip;
          break;

        case OP_TYPE_FLOAT:
          if (op0.floatLiteral >= op1.floatLiteral)
            eip = current.OperandList[2].instrIndex;
          else
            ++eip;
          break;
        }
      }
      break;

    case OP_JG:
      {
        assert(current.OperandList[2].type_id == OP_TYPE_INSTR_INDEX);

        Operand op0 = current.OperandList[0].fullyResolveOperand();
        Operand op1 = current.OperandList[1].fullyResolveOperand();

        switch (op0.type_id)
        {
        case OP_TYPE_INT:
        case OP_TYPE_INSTR_INDEX:
          if (op0.intLiteral > op1.intLiteral)
            eip = current.OperandList[2].instrIndex;
          else
            ++eip;
          break;

        case OP_TYPE_FLOAT:
          if (op0.floatLiteral > op1.floatLiteral)
            eip = current.OperandList[2].instrIndex;
          else
            ++eip;
          break;
        }
      }
      break;

    case OP_JL:
      {
        assert(current.OperandList[2].type_id == OP_TYPE_INSTR_INDEX);

        Operand op0 = current.OperandList[0].fullyResolveOperand();
        Operand op1 = current.OperandList[1].fullyResolveOperand();

        switch (op0.type_id)
        {
        case OP_TYPE_INT:
        case OP_TYPE_INSTR_INDEX:
          if (op0.intLiteral < op1.intLiteral)
            eip = current.OperandList[2].instrIndex;
          else
            ++eip;
          break;

        case OP_TYPE_FLOAT:
          if (op0.floatLiteral < op1.floatLiteral)
            eip = current.OperandList[2].instrIndex;
          else
            ++eip;
          break;
        }
      }
      break;   

    case OP_HALT:
      return;
      break;

    case OP_NEW:
      {
        if (current.OperandList[1].type_id == OP_TYPE_OBJECT_TAG)
        {
          tag_t classTag = current.OperandList[1].objectTag;
          ClassEntry& ce = ClassTable::GetSingleton().getClass(classTag);

          //allocate 
          Object* newObj = new Object;
          ObjectData* objData = (ObjectData*)malloc(ce.objectSize + OBJDATA_HEADER_SIZE);          
          
          //initialize
          objData->refCount = 0;
          newObj->data = objData;
          newObj->actualID = classTag;
          newObj->originalID = classTag;
          newObj->addRef();

          current.OperandList[0].storeOperand(newObj);
        }
        else if (current.OperandList[1].type_id == OP_TYPE_TYPE_TAG)
        {
          //for now, only strings
          assert(current.OperandList[1].typeTag == TYPE_STRING);
          Operand newStr;
          newStr.type_id = OP_TYPE_STRING;
          newStr.stringLiteralIdx = RTStringTable::GetSingleton().getNewEmptyString();
          current.OperandList[0].storeOperand(newStr);
        }
      }
      break;

    case OP_NEWA:
      {
        int arrayDim = current.OperandList[2].retrieveOperandAsInt();

        //oggetto contenitore
        ArrayObject* newArray = new ArrayObject;

        //allocate 
        newArray->arrayData = (char*)malloc(arrayDim * WORD_SIZE);

        //initialize
        newArray->dimension = arrayDim;

        if (current.OperandList[1].type_id == OP_TYPE_OBJECT_TAG)
        {
          tag_t classTag = current.OperandList[1].objectTag;
          newArray->actualID = classTag;
          newArray->originalID = classTag;
        }
        else if (current.OperandList[1].type_id == OP_TYPE_TYPE_TAG)
        {
          //array di tipi primitivi
          tag_t typeTag = current.OperandList[1].typeTag;
          newArray->actualID = typeTag;
          newArray->originalID = typeTag;
        }
        newArray->refCount = 1;

        Operand arrayRef;
        arrayRef.type_id = OP_TYPE_ARRAY;
        arrayRef.objAddr = newArray;

        current.OperandList[0].storeOperand(arrayRef);     
        
        //current.OperandList[0].storeOperand(newArray);     
      }
      break;

    case OP_LEA: 
      {
        //prende un operando oggetto(1), lo carica in (0)
        Operand  op = current.OperandList[1].resolveOperand(); 
        Operand address = addressOf(op);
        if (current.OperandList[0].type_id == OP_TYPE_REG)
          registers[current.OperandList[0].regCode] = address;
        else
          current.OperandList[0].storeOperand(address);        
      }
      break;

    case OP_LEAA:
      {
        //versione con 3 operandi, per array
        Operand  op = current.OperandList[1].resolveOperand();
        //dovrebbe essere un ArrayObject, quindi
        assert(/*OP_TYPE_OBJECT == op.type_id ||*/ OP_TYPE_ARRAY == op.type_id);
        int index = current.OperandList[2].retrieveOperandAsInt();
        
        ArrayObject* array = dynamic_cast<ArrayObject*>(op.objAddr);
        char* p = array->arrayData + (index * WORD_SIZE);
        current.OperandList[0].storeOperand(p);
      }
      break;

    case OP_PADD:
      {
        char* op1 = current.OperandList[1].retrieveOperandAsPointer();
        int op2 = current.OperandList[2].retrieveOperandAsInt();
        op1 += op2;
        current.OperandList[0].storeOperand(op1);
      }
      break;

    case OP_PSUB:
      {
        char* op1 = current.OperandList[1].retrieveOperandAsPointer();
        int op2 = current.OperandList[2].retrieveOperandAsInt();
        op1 -= op2;
        current.OperandList[0].storeOperand(op1);
      }
      break;

    case OP_STORE:
      {
        //maybe not even resolve the pointer, here...

        Operand op1 = current.OperandList[1].retrieveOperandAsOperand();
        Operand memoryLocation = current.OperandList[0].retrieveOperandAsOperand();
        
        //pointer -> object member and then write it
        if (op1.type_id == OP_TYPE_OBJECT  || 
            op1.type_id == OP_TYPE_ARRAY   ||
            op1.type_id == OP_TYPE_POINTER ||
            op1.type_id == OP_TYPE_STRING )
          memoryLocation.storeOperand(op1);
        else
          assert(0);
      }
      break;

    case OP_DEREF:
      {
        Operand value;
        Operand op1;
        Operand memoryLocation = current.OperandList[0].retrieveOperandAsOperand();

        if (current.OperandList[1].type_id != OP_TYPE_POINTER)
        {
          op1 = current.OperandList[1].resolveOperand();
        }
        else 
        {
          op1 = current.OperandList[1];
        }

        if (op1.type_id == OP_TYPE_POINTER)
          value = op1.resolveOperand();
        else
          value = op1;


        assert(current.OperandList[0].type_id == OP_TYPE_REG || 
          current.OperandList[0].type_id == OP_TYPE_REL_STACK_INDEX ||
          current.OperandList[0].type_id == OP_TYPE_ABS_STACK_INDEX);
        switch (current.OperandList[0].type_id)
        {
        case OP_TYPE_REG:
          registers[current.OperandList[0].regCode] = value;
          break;
        case OP_TYPE_ABS_STACK_INDEX:
          setStackValue(current.OperandList[0].absStackIndex, value);
          break;

        case OP_TYPE_REL_STACK_INDEX:
          {
            int absIndex = registers[REG_EBP].intLiteral + current.OperandList[0].relStackIndex;
            setStackValue(absIndex, value);
            break;
          }
        }
      }
      break;

    case OP_RELEASE:
      {
        Operand op = current.OperandList[0].resolveOperand();
        assert(op.type_id == OP_TYPE_OBJECT || op.type_id == OP_TYPE_ARRAY);

        if (op.objAddr == (Object*)BAD_PTR)
          break;

        op.objAddr->release();

        if (op.type_id == OP_TYPE_OBJECT)
        {
          delete op.objAddr;
          //op.objAddr = NULL;
          current.OperandList[0].storeOperand((Object*)NULL);
        }
      }
      break;

    case OP_DCAST:
      {
        Operand op1 = current.OperandList[1].fullyResolveOperand();
       
        Operand ret = op1;

        tag_t newTypeID = current.OperandList[2].objectTag;
        assert(op1.type_id == OP_TYPE_OBJECT);

        if (ClassTable::GetSingleton().isDowncastPossible(newTypeID, op1.objAddr->originalID))
        {
          //we must check before doing such a thing!
          ret.objAddr->actualID = newTypeID;
        }
        else
        {
          //raise exception
        }

        current.OperandList[1].storeOperand(ret);
      }
      break;

    case OP_UCAST:
      {
        Operand op1 = current.OperandList[1].fullyResolveOperand();

        Operand ret = op1;

        tag_t newTypeID = current.OperandList[2].objectTag;
        assert(op1.type_id == OP_TYPE_OBJECT);
       
        //this is an upcast, checking already done by compiler
        ret.objAddr->actualID = newTypeID;
        current.OperandList[1].storeOperand(ret);
      }
      break;

    case OP_STR2I:
      {
        Operand op = current.OperandList[1].fullyResolveOperand();
        assert(op.type_id == OP_TYPE_STRING);

        int i = atoi(RTStringTable::GetSingleton().getString(op.stringLiteralIdx).c_str());
        current.OperandList[0].storeOperand(i);
      }
      break;

    case OP_STR2F:
      {
        Operand op = current.OperandList[1].fullyResolveOperand();
        assert(op.type_id == OP_TYPE_STRING);

        float f = (float)atof(RTStringTable::GetSingleton().getString(op.stringLiteralIdx).c_str());
        current.OperandList[0].storeOperand(f);      
      }
      break;

    case OP_I2STR:
      {
        int intValue = current.OperandList[1].retrieveOperandAsInt();
        int index = RTStringTable::GetSingleton().getNewEmptyString();

        std::ostringstream ss;
        ss << intValue;

        RTStringTable::GetSingleton().setString(index, ss.str());
        current.OperandList[0].storeStringOperand(index);
      }
      break;

    case OP_F2STR:
      {
        float floatValue = current.OperandList[1].retrieveOperandAsFloat();
        int index = RTStringTable::GetSingleton().getNewEmptyString();

        std::ostringstream ss;
        ss << floatValue;

        RTStringTable::GetSingleton().setString(index, ss.str());
        current.OperandList[0].storeStringOperand(index);
      }
      break;

    case OP_STRAT:
      {
        Operand op1 = current.OperandList[1].fullyResolveOperand();
        assert(op1.type_id == OP_TYPE_STRING);
        int index = current.OperandList[1].retrieveOperandAsInt();

        int newString = RTStringTable::GetSingleton().getNewEmptyString();
        std::string s = RTStringTable::GetSingleton().getString(op1.stringLiteralIdx);
        std::string news = s.substr(index, 1);
        RTStringTable::GetSingleton().setString(newString, news);
      }
      break;

    case OP_STRAPPEND:
      {
        //TODO
      }
      break;

    default:
      //error

      break;
    }

    if (debug)
    {
      fprintf(stderr, "%i : ", oldEip);
      current.dump();

      if (current.OpCode == OP_RET || current.OpCode == OP_CALL)
      {
        std::cout << "returned: ";
        registers[REG_RET].dump();
      }
      /*
      std::string s;
      std::cin >> s;
      
      if (s == "s")
      */
        dumpStack();
    /*  
    else if (s == "f")
        dumpCurrentFrame();
      //else if (s == "r")
        //run
      else if (s == "x")
        break;
        */
    }

    if (oldEip == eip)
      ++eip;
  }
}

void VirtualMachine::setStackSize(size_t size)
{
  //Check if reasonable size
  range(256U, size, 16348U);
  if (Stack) delete[] Stack;

  Stack = new Operand[size];
}


void VirtualMachine::dumpStack()
{
  for (int i = registers[REG_ESP].intLiteral; i >= 0; --i)
  {
    Stack[i].dump();
    if (i == registers[REG_ESP].intLiteral)
      std::cout << " <- ESP";
    if (i == registers[REG_EBP].intLiteral)
      std::cout << " <- EBP";
    std::cout << std::endl;
  }
}

void VirtualMachine::dumpCurrentFrame()
{
  int bottom = registers[REG_EBP].intLiteral - STACK_PARAMS_OFFSET;
  for (int i = registers[REG_ESP].intLiteral; i >= bottom; --i)
  {
    Stack[i].dump();
    if (i == registers[REG_ESP].intLiteral)
      std::cout << " <- ESP";

    switch (i - registers[REG_EBP].intLiteral)
    {
    case STACK_OLD_EBP_OFFSET:
      std::cout << " <- saved EBP";
      break;
    case STACK_LOCALS_OFFSET:
      std::cout << " <- LOCALS (EBP)";
      break;
    case STACK_RA_OFFSET:
      std::cout << " <- RETURN ADDRESS";
      break;
    case STACK_THIS_OFFSET:
      std::cout << " <- THIS";
      break;
    case STACK_PARAMS_OFFSET:
      std::cout << " <- PARAMS";
      break;
    }

    std::cout << std::endl;
  }
}


VirtualMachine VirtualMachineSingleton;

