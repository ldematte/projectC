
#ifndef CODE_LOADER_H_INCLUDED_
#define CODE_LOADER_H_INCLUDED_

#include "../common.h"
#include "../constants.h"
#include "../CodeHeader.h"
#include "../Instruction.h"


class CodeLoader
{
public:

  bool load(char* FileName);
  void unload();

  Instruction* getAddress() { return codeStream; }
  size_t getStackSize() { return h.stackSize; } 
  size_t getStreamSize() { return (h.objectsOffset - h.textOffset) / sizeof(Instruction); } 

private:

  Instruction* codeStream;
  Header h;
 
};

#endif //CODE_LOADER_H_INCLUDED_


