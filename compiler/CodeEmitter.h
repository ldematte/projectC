

#ifndef CODE_EMITTER_H_INCLUDED_
#define CODE_EMITTER_H_INCLUDED_

#include <string>
#include <stdio.h>

#include <Singleton.h>
#include "../CodeHeader.h"
#include "symbol.h"

class CodeEmitter : public Singleton<CodeEmitter>
{
private:
  FILE* f;
  Header h;

  int currentCodeOffset;
  int currentFileOffset;

  void computeFunctionSize(SymPtr sym);
  //int  computeLocalSize(SymPtr sym);
  void injectPrologEpilog(SymPtr sym);
  //void indexVariables(SymPtr sym, int paramIndex = 0, int localIndex = 0);

  int findEntryPoint();
  tag_t findGlobalTag();

  void emitClassRec(SymPtr sym);
  void insertGlobalInit(SymPtr global);

public:
  CodeEmitter()
  {
    f = NULL;
    currentCodeOffset = 0;
  }

  bool prepare();

  bool createOutput(const std::string& filename);

  bool emitHeader();
  bool emitStringTable();
  bool emitCode();
  bool emitObjectTable();


  bool emitClass(SymPtr sym);  
  bool emitSymCode(SymPtr sym);

  
};


#endif //CODE_EMITTER_H_INCLUDED_


