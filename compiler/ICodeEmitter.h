
//The intermediate code generator

#ifndef ICODE_EMITTER_H_INCLUDED_
#define ICODE_EMITTER_H_INCLUDED_

#include <list>
#include <string>
#include <map>
#include <Instruction.h>
#include <Singleton.h>

#include "MyStack.h"
#include "symbol.h"

enum CODE_NODE
{
  INSTRUCTION = 0,
  JUMP_TARGET
};

//classe base 
struct ICodeNode
{
  //union
  Instruction instr;
  int jumpTarget;
  //union

  CODE_NODE tag;

  void dump(FILE* f)
  {
    if (tag == JUMP_TARGET)
      fprintf(f, "%i: \n", jumpTarget);
    else
      instr.dump(f);
  }
};

class ICodeEmitter : public Singleton<ICodeEmitter>
{
private:

  typedef std::list<ICodeNode> ICodeStream;
  typedef std::map< SymPtr, ICodeStream > code_map;

  code_map code;

  typedef std::list<ICodeNode>::iterator stream_iter;

  typedef std::map< SymPtr, ICodeStream >::iterator iterator;

public:
  /*
  struct LoopBookmark
  {
    //coppia di iteratori, jump target iniziale 
    //e jump target finale del loop
    
    ICodeNode startTarget;
    ICodeNode endTarget;

    LoopBookmark(const ICodeNode& start_target, const ICodeNode& end_target)
      : startTarget(start_target), endTarget(end_target) { }
  };
  
  struct SwitchBookmark
  {
    //coppia di iteratori, prima istruzione 
    //e jump target finale dello switch
    stream_iter firstInstr;
    ICodeNode   endTarget;

    SwitchBookmark(stream_iter first_instr, const ICodeNode& end_target)
      : firstInstr(first_instr), endTarget(end_target) { }
  };
  */

  struct Bookmark
  {
    //coppia di iteratori, prima istruzione 
    //e jump target finale dello switch
    stream_iter firstInstr;
    ICodeNode   endTarget;

    //added
    bool hasDefault;

    Bookmark(stream_iter first_instr, const ICodeNode& end_target)
      : firstInstr(first_instr), endTarget(end_target), hasDefault(false) { }
  };

  MyStack<Bookmark> loopStack;

private:
  
  int nextJumpLabel;
  stream_iter currentPosition;
  int currentOperand;

public:
  ICodeEmitter() { nextJumpLabel = 0; currentOperand = 0; } 

  void addInstr(OPCODE op);
  void addInstr(OPCODE op, stream_iter here);
  void setStreamPosition(stream_iter here);

  //adds an operand to the last opcode
  void addOperand(int i);  
  void addOperand(float f);
  void addOperand(bool b);

  //for the I-code, it's OK to have not fully
  //resolved references
  void addOperand(SymPtr sym);
  void addOperand(TypePtr type);

  //we need to build a constant table here and make 
  //string-to-index conversion.
  //(in the stream there will be the index)
  void addOperand(const std::string& s);
  void addOperand(const Operand& op);

  void addRegOperand(int reg_code);

  ICodeNode generateJumpTarget();

  void addJumpTarget(ICodeNode target);

  void pushLoop(const stream_iter& loopBegin, const ICodeNode& loopEnd);
  void popLoop();
  Bookmark& getCurrentLoop();

  stream_iter getCurrentInstr();

  void addOperand(Object* o);
  void addOperand(char* pointer);
  void addOperand(TYPE_TYPE typeTag);

  void dump(FILE* f = stderr);

  ICodeStream& getStreamForFunction(SymPtr sym);

  void   ICodeStream_resolveLabels(SymPtr sym);
  size_t ICodeStream_getSize(SymPtr sym);
  size_t ICodeStream_getStreamSize(SymPtr sym);
  void   ICodeStream_optimize(std::list<ICodeNode>& codeStream);
  std::list<ICodeNode>::iterator ICodeStream_findLabel(std::list<ICodeNode>& codeStream, int jumpLabel);

  bool ICodeStream_appendEpilog(SymPtr sym);
  bool ICodeStream_insertProlog(SymPtr sym);

  void optimize();

  iterator begin()
  {
    return (code.begin());
  }

  iterator end()
  {
    return (code.end());
  }

};

//macro per facilitare la scrittura
template <class T1, class T2, class T3>
inline void xasm (OPCODE x, const T1& a, const T2& b, const T3& c) 
{ 
  ICodeEmitter::GetSingleton().addInstr(x);
  ICodeEmitter::GetSingleton().addOperand(a);
  ICodeEmitter::GetSingleton().addOperand(b);
  ICodeEmitter::GetSingleton().addOperand(c);
}

template <class T1, class T2>
inline void xasm (OPCODE x, const T1& a, const T2& b) 
{ 
  ICodeEmitter::GetSingleton().addInstr(x);
  ICodeEmitter::GetSingleton().addOperand(a);
  ICodeEmitter::GetSingleton().addOperand(b);
}

template <class T1>
inline void xasm (OPCODE x, const T1& a) 
{ 
  ICodeEmitter::GetSingleton().addInstr(x);
  ICodeEmitter::GetSingleton().addOperand(a);
}

inline void xasm(OPCODE x)
{
  ICodeEmitter::GetSingleton().addInstr(x);
}




#endif //ICODE_EMITTER_H_INCLUDED_
