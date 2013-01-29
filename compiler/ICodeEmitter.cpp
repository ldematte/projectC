
#include "ICodeEmitter.h"
#include "StringTable.h"
#include "ScopeManager.h"

#include "../constants.h"

#include <iostream>

#define string_table StringTable::GetSingleton()


void ICodeEmitter::addInstr(OPCODE op, stream_iter here)
{
  ICodeNode node;
  node.instr.OpCode = op;
  node.tag = INSTRUCTION;
  
  std::list<ICodeNode>& codeStream = code[ScopeManager::GetSingleton().getCurrentScope()];
  currentPosition = codeStream.insert(here, node); 

  currentOperand = 0;
}

void ICodeEmitter::addInstr(OPCODE op)
{
  ICodeNode node;
  node.instr.OpCode = op;
  node.tag = INSTRUCTION;

  
  std::list<ICodeNode>& codeStream = code[ScopeManager::GetSingleton().getCurrentScope()];

  codeStream.push_back(node);  
  currentPosition = prior(codeStream.end());

  currentOperand = 0;
}

void ICodeEmitter::addJumpTarget(ICodeNode target)
{
  std::list<ICodeNode>& codeStream = code[ScopeManager::GetSingleton().getCurrentScope()];

  target.tag = JUMP_TARGET;
  codeStream.push_back(target);
  currentOperand = 0;
}

void ICodeEmitter::addOperand(const std::string& s)
{
  assert(currentOperand < 3);

  int idx = string_table.insert(s);
  Operand op;
  op.stringLiteralIdx = idx;
  op.type_id = OP_TYPE_STRING;
  currentPosition->instr.OperandList[currentOperand] = op;
  
  ++currentOperand;
}

void ICodeEmitter::addOperand(int i)
{
  assert(currentOperand < 3);

  Operand op;
  op.intLiteral = i;
  op.type_id = OP_TYPE_INT;
  currentPosition->instr.OperandList[currentOperand] = op;
  
  ++currentOperand;
}

void ICodeEmitter::addOperand(const Operand& op)
{
  assert(currentOperand < 3);

  currentPosition->instr.OperandList[currentOperand] = op;
  
  ++currentOperand;
}

void ICodeEmitter::addOperand(float f)
{
  assert(currentOperand < 3);

  Operand op;
  op.floatLiteral = f;
  op.type_id = OP_TYPE_FLOAT;
  currentPosition->instr.OperandList[currentOperand] = op;
  
  ++currentOperand;
}

void ICodeEmitter::addOperand(bool b)
{
  assert(currentOperand < 3);

  Operand op;
  op.intLiteral = (b ? 1 : 0);
  op.type_id = OP_TYPE_INT;
  currentPosition->instr.OperandList[currentOperand] = op;
  
  ++currentOperand;
}

void ICodeEmitter::addOperand(SymPtr symbol)
{
  assert(currentOperand < 3);

  Operand op;
  op.symbol = symbol;
  op.type_id = OP_TYPE_SYMBOL;
  currentPosition->instr.OperandList[currentOperand] = op;
  
  ++currentOperand;
}

void ICodeEmitter::addOperand(TYPE_TYPE typeTag)
{
  assert(currentOperand < 3);

  Operand op;
  op.typeTag = typeTag;
  op.type_id = OP_TYPE_TYPE_TAG;
  currentPosition->instr.OperandList[currentOperand] = op;
  
  ++currentOperand;
}

void ICodeEmitter::addOperand(TypePtr type)
{
  assert(currentOperand < 3);

  Operand op;
  op.type = type;
  op.type_id = OP_TYPE_TYPE;
  currentPosition->instr.OperandList[currentOperand] = op;
  
  ++currentOperand;
}

void ICodeEmitter::addRegOperand(int reg_code)
{
  assert(currentOperand < 3);

  Operand op;
  op.regCode = reg_code;
  op.type_id = OP_TYPE_REG;
  currentPosition->instr.OperandList[currentOperand] = op;
  
  ++currentOperand;  
}

ICodeNode ICodeEmitter::generateJumpTarget()
{
  ICodeNode node;
  node.tag = JUMP_TARGET;
  node.jumpTarget = ++nextJumpLabel;

  return node;
}

ICodeEmitter::stream_iter ICodeEmitter::getCurrentInstr()
{ 
  return currentPosition;
}

ICodeEmitter::Bookmark& ICodeEmitter::getCurrentLoop()
{
  return loopStack.top();
}

void ICodeEmitter::popLoop()
{
  loopStack.pop();
}

void ICodeEmitter::pushLoop(const ICodeEmitter::stream_iter& loopBegin, const ICodeNode& loopEnd)
{
  loopStack.push(ICodeEmitter::Bookmark(loopBegin, loopEnd));
}

void ICodeEmitter::setStreamPosition(ICodeEmitter::stream_iter here)
{
  currentPosition = here;
}

void ICodeEmitter::ICodeStream_resolveLabels(SymPtr sym)
{
  std::map<int, int> labelMap;

  std::list<ICodeNode>& codeStream = code[sym];
  std::list<ICodeNode>::iterator it;

  int instrAddress = sym->functionOffset;

  //prima passata: memorizza a che posizione sono le etichette
  for (it = codeStream.begin(); it != codeStream.end(); ++it)
  {
    if (it->tag == JUMP_TARGET)
      labelMap[it->jumpTarget] = instrAddress;
    else
      ++instrAddress;
  }

  for (it = codeStream.begin(); it != codeStream.end();)
  {
    if (it->tag == JUMP_TARGET)
    {
      it = codeStream.erase(it);
    }
    else
    {
      int idx = it->instr.jumpIndex();
      if (idx != -1)
      {
        int oldAddr = it->instr.OperandList[idx].instrIndex;
        it->instr.OperandList[idx].instrIndex = labelMap[oldAddr];
        it->instr.OperandList[idx].type_id = OP_TYPE_INSTR_INDEX;
      }
      ++it;
    }
  } 
}

void ICodeEmitter::dump(FILE* f)
{
  code_map::iterator map_it;

  for (map_it = code.begin(); map_it != code.end(); ++map_it)
  {
    fprintf(f, "SCOPE: %s", map_it->first->getQualifiedName().c_str());
    fprintf(f, "%s\n", (map_it->first->isVirtual ? " VIRTUAL" : ""));
    fprintf(f, "[\n");
    std::list<ICodeNode>& codeStream = map_it->second;
    std::list<ICodeNode>::iterator it;
    for (it = codeStream.begin(); it != codeStream.end(); ++it)
      it->dump(f);
    fprintf(f, "]\n");
  }
}

void ICodeEmitter::optimize()
{
  code_map::iterator map_it;

  for (map_it = code.begin(); map_it != code.end(); ++map_it)
  {
    std::list<ICodeNode>& codeStream = map_it->second;
    ICodeStream_optimize(codeStream);
  }  
}

bool removeDuplicate(OPCODE op, std::list<ICodeNode>::iterator& it, std::list<ICodeNode>& codeStream)
{
  //stessa istruzione 2 volte di fila (OP_MOV)
  if (it->instr.OpCode == op)
  {
    std::list<ICodeNode>::iterator it2 = it;
    //scorro lo stream fino a quando trovo un'altra
    //istruzione
    while ((++it2)->tag == JUMP_TARGET);

    if ( (it2->instr.OpCode == op) &&
         (it->instr.OperandList[0].isEqual(it2->instr.OperandList[0])) &&
         (it->instr.OperandList[1].isEqual(it2->instr.OperandList[1])) )
    {
      codeStream.erase(it);
      it = it2;
    }             
  }     

  //optimization possible on a single node:
  if (it->instr.OpCode == op)
    if (it->instr.OperandList[0].isEqual(it->instr.OperandList[1]))
    {
      it = codeStream.erase(it);
      return true;
    }

  return false;
}

bool removeZeroOp(OPCODE op, std::list<ICodeNode>::iterator& it, std::list<ICodeNode>& codeStream)
{
  //istr con un operand a zero
  //(solo 3-opcode)
  if (it->instr.OpCode == op)
  {
    if (it->instr.OperandList[1].type_id == OP_TYPE_INT && 
        it->instr.OperandList[1].intLiteral == 0)
    {
      it = codeStream.erase(it);
      return true;
    }

    if (it->instr.OperandList[2].type_id == OP_TYPE_INT && 
        it->instr.OperandList[2].intLiteral == 0)
    {
      it = codeStream.erase(it);
      return true;
    }

    if (it->instr.OperandList[1].type_id == OP_TYPE_FLOAT && 
        it->instr.OperandList[1].floatLiteral == 0.0f)
    {
      it = codeStream.erase(it);
      return true;
    }

    if (it->instr.OperandList[2].type_id == OP_TYPE_FLOAT && 
        it->instr.OperandList[2].floatLiteral == 0.0f)
    {
      it = codeStream.erase(it);
      return true;
    }
  }    

  return false;
} 

void ICodeEmitter::ICodeStream_optimize(std::list<ICodeNode>& codeStream)
{
  std::list<ICodeNode>::iterator it;
  for (it = codeStream.begin(); it != codeStream.end();)
  {
    if (it->tag == INSTRUCTION)
    {
      if (removeDuplicate(OP_MOV, it, codeStream))
        continue;
      if (removeDuplicate(OP_SET, it, codeStream))
        continue;
      if (removeZeroOp(OP_PADD, it, codeStream))
        continue;
      if (removeZeroOp(OP_ADD, it, codeStream))
        continue;

      int jumpIdx = (*it).instr.jumpIndex();
      if (jumpIdx != -1)
      {
        bool found = false;
        std::list<ICodeNode>::iterator it2 = ICodeStream_findLabel(codeStream, (*it).instr.OperandList[jumpIdx].instrIndex);
        while (it2 != codeStream.end())
        {
          if (it2->tag == JUMP_TARGET)
            ++it2;
          else
          {
            found = true;
            break;
          }
        }

        if (found)
        {
          //un'altra istruzione di jump
          int jumpIdx2 = (*it2).instr.jumpIndex();
          if (jumpIdx2 != -1)
          {
            switch (it2->instr.OpCode)
            {
            case OP_JMP:
              it->instr.OperandList[jumpIdx] = it2->instr.OperandList[jumpIdx2];
              break;

            case OP_JZ:
              if (it->instr.OpCode == OP_JZ && 
                  (it->instr.OperandList[0].isEqual(it2->instr.OperandList[0])))
              {
                //stesso jump 2 volte
                it->instr.OperandList[jumpIdx] = it2->instr.OperandList[jumpIdx2];
              }
              break;

            case OP_JNZ:
              if (it->instr.OpCode == OP_JNZ && 
                  (it->instr.OperandList[0].isEqual(it2->instr.OperandList[0])))
              {
                //stesso jump 2 volte
                it->instr.OperandList[jumpIdx] = it2->instr.OperandList[jumpIdx2];
              }
              break;
            }
          }
        }
      }
    }
    ++it;
  }
}

std::list<ICodeNode>::iterator ICodeEmitter::ICodeStream_findLabel(std::list<ICodeNode>& codeStream, int jumpLabel)
{
  std::list<ICodeNode>::iterator it;
  for (it = codeStream.begin(); it != codeStream.end(); ++it)
  {
    if (it->tag == JUMP_TARGET)
      if (it->jumpTarget == jumpLabel)
        return it;
  }
  return it;
}

ICodeEmitter::ICodeStream& ICodeEmitter::getStreamForFunction(SymPtr sym)
{  
  std::list<ICodeNode>& codeStream = code[sym];
  return codeStream;
}

size_t ICodeEmitter::ICodeStream_getStreamSize(SymPtr sym)
{  
  std::list<ICodeNode>& codeStream = code[sym];
  return (codeStream.size() /* sizeof(Instruction)*/);
}

size_t ICodeEmitter::ICodeStream_getSize(SymPtr sym)
{
  //std::list<ICodeNode>& codeStream = code[ScopeManager::GetSingleton().getCurrentScope()];
  std::list<ICodeNode>& codeStream = code[sym];
  size_t instrSize = 1; //sizeof(Instruction)
  size_t size = 0;
  
  std::list<ICodeNode>::iterator it;
  for (it = codeStream.begin(); it != codeStream.end(); ++it)
  {
    if (it->tag == INSTRUCTION)
      size += instrSize;
  }
  return size;
}

bool ICodeEmitter::ICodeStream_appendEpilog(SymPtr sym)
{
  assert(sym->symKind == SYM_FUNCTION);

  std::list<ICodeNode>& codeStream = code[sym];
  std::list<ICodeNode> epilog;

  ICodeNode node;
  Operand op;
  node.tag = INSTRUCTION;

  //la VM deve eseguire questo prima di saltare dopo una RET  
  //va iniettato dappertutto
  std::list<SymPtr>::iterator it;

  //Start epilog
  //NOP
  //node.instr.OpCode = OP_NOP;
  //epilog.push_back(node);

  Operand currVar;
  currVar.type_id = OP_TYPE_REL_STACK_INDEX;

  //locals
  /*
  int num = STACK_LOCALS_OFFSET;
  for (it = sym->declList.begin(); it != sym->declList.end(); ++it, ++num)
  {
    //rilascia gli oggetti locali
    SymPtr localSym = (*it);
    if ((localSym->symKind == SYM_VAR) && !localSym->isArgument       
      && !(localSym->type->isPrimitive()))
    {
      currVar.relStackIndex = num;
      node.instr.OpCode = OP_RELEASE;
      node.instr.OperandList[0] = currVar;
      epilog.push_back(node);
    }
  }

  //params
  int i;
  for (i = 0; i < (int)(sym->type->getParamCount()); ++i)
  {
    if (!(sym->type->typeList[i]->isPrimitive()))
    {
      int idx = -i + STACK_PARAMS_OFFSET;
      currVar.relStackIndex = idx;
      node.instr.OpCode = OP_RELEASE;
      node.instr.OperandList[0] = currVar;
      epilog.push_back(node);
    }
  }  
  */

  for (it = sym->declList.begin(); it != sym->declList.end(); ++it)
  {
    //rilascia gli oggetti locali e argomenti
    SymPtr localSym = (*it);
    if ((localSym->symKind == SYM_VAR) && !(localSym->type->isPrimitive()))
    {
      currVar.relStackIndex = localSym->getStackOffset();
      node.instr.OpCode = OP_RELEASE;
      node.instr.OperandList[0] = currVar;
      epilog.push_back(node);
    }
  }

  //this
  if (sym->isMember)
  {
    //we have a this pointer
    currVar.relStackIndex = STACK_THIS_OFFSET;
    node.instr.OpCode = OP_RELEASE;
    node.instr.OperandList[0] = currVar;
    epilog.push_back(node);
  }

  

  //MOV ESP, EBP
  node.instr.OpCode = OP_MOV;
  op.regCode = REG_ESP;
  op.type_id = OP_TYPE_REG;
  node.instr.OperandList[0] = op;
  op.regCode = REG_EBP;
  node.instr.OperandList[1] = op;
  epilog.push_back(node);

  //POP EBP
  node.instr.OpCode = OP_POP;
  op.regCode = REG_EBP;
  op.type_id = OP_TYPE_REG;
  node.instr.OperandList[0] = op;
  epilog.push_back(node);

  //NOP
  //node.instr.OpCode = OP_NOP;
  //epilog.push_back(node);
  //End epilog

  //clear the stack from params
  /*
  node.instr.OpCode = OP_SUB;
  op.regCode = REG_EBP;
  op.type_id = OP_TYPE_REG;
  node.instr.OperandList[0] = op;
  node.instr.OperandList[1] = op;
  op.type_id = OP_TYPE_INT;
  op.intLiteral = sym->getParamCount() + (sym->isMember ? 1 : 0);
  node.instr.OperandList[2] = op;
  epilog.push_back(node);
  */

  //append a RET instr in the end(if not present)
  if (prior(codeStream.end())->instr.OpCode != OP_RET)
  {
    node.instr.OpCode = OP_RET;
    //RA and (eventually) this
    node.instr.OperandList[0].intLiteral = sym->getParamCount() + (sym->isMember ? 2 : 1);
    node.instr.OperandList[0].type_id = OP_TYPE_INT;
    codeStream.push_back(node);
  }

  //scan the codeStream for RET instruction(s)
  std::list<ICodeNode>::iterator codeIt;
  for (codeIt = codeStream.begin(); codeIt != codeStream.end();)
  {
    if (codeIt->instr.OpCode == OP_RET)
    {
      //if main, HALT
      if (sym->name == "main")
        codeIt->instr.OpCode = OP_HALT;
 
      codeStream.insert(codeIt, epilog.begin(), epilog.end());
    }
    ++codeIt;
  }

  

  return true;  
}

bool ICodeEmitter::ICodeStream_insertProlog(SymPtr sym)
{
  std::list<ICodeNode>& codeStream = code[sym];

  ICodeNode node;
  Operand op;
  node.tag = INSTRUCTION;
  
  std::list<ICodeNode> prolog;


  //PUSH EBP
  node.instr.OpCode = OP_PUSH;
  op.regCode = REG_EBP;
  op.type_id = OP_TYPE_REG;
  node.instr.OperandList[0] = op;
  prolog.push_back(node);

  //MOV EBP, ESP
  node.instr.OpCode = OP_MOV;
  op.regCode = REG_EBP;
  op.type_id = OP_TYPE_REG;
  node.instr.OperandList[0] = op;
  op.regCode = REG_ESP;
  node.instr.OperandList[1] = op;
  prolog.push_back(node);

  //ADD ESP, size <- NB: non SUB come su Wintel!
  node.instr.OpCode = OP_ADD;
  op.regCode = REG_ESP;
  op.type_id = OP_TYPE_REG;
  node.instr.OperandList[0] = op;
  node.instr.OperandList[1] = op;
  op.intLiteral = sym->getLocalCount();
  op.type_id = OP_TYPE_INT;
  node.instr.OperandList[2] = op;
  prolog.push_back(node);
    

  codeStream.insert(codeStream.begin(), prolog.begin(), prolog.end());
  
  return true;
}

ICodeEmitter ICodeEmitterSingleton;


