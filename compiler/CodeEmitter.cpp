
#include "CodeEmitter.h"
#include "StringTable.h"
#include "SymbolTable.h"
#include "ICodeEmitter.h"
#include "ErrorManager.h"
#include "Native.h"

#define IE ICodeEmitter::GetSingleton()

#include <vector>
#include <string>
#include "dynarray.hpp"

//layout:
//header
//string table
//code <- si memorizzano gli entry point (functionOffset)
//object table
//  function table
//si deve ripassare a sistemare i call? no, restano (CLASSE, ORDINALE)

//si puo' assegnare l'ordinale alla funzione quando si 
//inserisce nella symbol table?

//e magari dare sempre zero al __global namespace

struct ClassTable
{
  int size; //or number of classes
  char* data;
};

struct ClassEntry
{
  int size; //la grandezza totale della struttura
  int objectSize;
  int functionCount;
  int virtualCount;
  //vtable;
  //functionTable;
};

struct FunctionTable
{
  int size; //or number of functions
  char* data;
};


/*
emitHeader();
emitStringTable();
emitCode();
emitObjectTable();

updateHeader();
*/

int CodeEmitter::findEntryPoint()
{
  Ident id;
  if (SymbolTable::GetSingleton().lookupIdSymbol("main", id))
  {
    if (id.symDefList.size() == 1)
    {
      int index = (*id.symDefList.begin())->functionOffset;
      return index;
    }
    else
    {
      ErrorManager::GetSingleton().logAndRaise("Entry point function (main) defined more than once");
    }
  }
  //raise error
  ErrorManager::GetSingleton().logAndRaise("Entry point function (main) not defined");
  return -1;
}

tag_t CodeEmitter::findGlobalTag()
{
  return reinterpret_cast<tag_t>(SymbolTable::GetSingleton().getGlobalNamespace());
}

bool CodeEmitter::emitHeader()
{
  fseek(f, 0, SEEK_SET);

  h.CRC = 0;
  h.entryPointIndex = findEntryPoint();
  h.globalTag = findGlobalTag();
  h.globalDataSize = SymbolTable::GetSingleton().getGlobalNamespace()->getMemberCount();
  
  h.magic = 0x00415246;
  h.versionMajor = 0;
  h.versionMinor = 80;
  h.stackSize = 16384;
  h.CRC = 0xFFFFFFFF;


  if (fwrite((void*)&h, sizeof(Header), 1, f))
  {
    currentFileOffset = sizeof(Header);
    return true;
  }
  else
  {
    currentFileOffset = 0;
    return false;
  }
}


bool CodeEmitter::createOutput(const std::string& filename)
{
  currentFileOffset = 0;
  if ( (f = fopen(filename.c_str(), "wb")) != 0 )
    return true;
  return false;
}

bool CodeEmitter::emitStringTable()
{
  size_t size;
  dynarray buf;

  for (size_t i = 0; i < StringTable::GetSingleton().Table.size(); ++i)
  {
    int str_length = (int)StringTable::GetSingleton().Table[i].size();
    buf.insert(str_length);

    buf.insert(StringTable::GetSingleton().Table[i].c_str(), (int)StringTable::GetSingleton().Table[i].size());
  }

  //write the symbol table on file
  size = buf.size();
  fwrite(&size, sizeof(size_t), 1, f);
  buf.dump(f);

  currentFileOffset = (int)(currentFileOffset + sizeof(size_t) + size);
  h.textOffset = currentFileOffset;

  return true;
}

bool CodeEmitter::emitCode()
{
  prepare();
  SymPtr global = SymbolTable::GetSingleton().getGlobalNamespace();
  emitSymCode(global);

  h.objectsOffset = currentFileOffset;
  return true;
}

bool CodeEmitter::emitObjectTable()
{
  SymPtr global = SymbolTable::GetSingleton().getGlobalNamespace();
  emitClassRec(global);

  h.fileSize = currentFileOffset;
  return true;
}

void CodeEmitter::computeFunctionSize(SymPtr sym)
{
  //adesso calcoliamo la lunghezza di ogni funzione
  //e la scriviamo

  //calcoliamo anche la dimensione di ogni classe
  //e la dimensione totale delle funzioni nella classe
  std::list<SymPtr>::iterator it;
  sym->functionCodeSize = 0;
  for (it = sym->declList.begin(); it != sym->declList.end(); ++it)
  {
    if ((*it)->symKind == SYM_FUNCTION)
    {
      int size = (int)ICodeEmitter::GetSingleton().ICodeStream_getSize(*it);
      int sizeInBytes = size * sizeof(Instruction);
      (*it)->functionCodeSize = size;
      (*it)->functionOffset = currentCodeOffset;
      currentCodeOffset += size;  
      currentFileOffset += sizeInBytes;

      //we use this member for the size of code
      //in a class. It may be useful in the future
      sym->functionCodeSize += size;
    }
    else if ((*it)->symKind == SYM_CLASS)
    {
      computeFunctionSize(*it);
    }
  }  
}


void CodeEmitter::injectPrologEpilog(SymPtr sym)
{
  std::list<SymPtr>::iterator it;
  for (it = sym->declList.begin(); it != sym->declList.end(); ++it)
  {
    if ((*it)->symKind == SYM_FUNCTION && !((*it)->isNative))
    {
      ICodeEmitter::GetSingleton().ICodeStream_insertProlog(*it);
      ICodeEmitter::GetSingleton().ICodeStream_appendEpilog(*it);
    }
    else if ((*it)->symKind == SYM_CLASS)
    {
      injectPrologEpilog(*it);
    }
  }  
}

void CodeEmitter::insertGlobalInit(SymPtr global)
{
  Ident id;
  if (SymbolTable::GetSingleton().lookupIdSymbol("main", id))
  {
    if (id.symDefList.size() == 1)
    {
      SymPtr sym = id.symDefList.front();
      assert(sym->parent == global);

      std::list<ICodeNode>& mainStream = ICodeEmitter::GetSingleton().getStreamForFunction(sym);
      std::list<ICodeNode>& globalStream = ICodeEmitter::GetSingleton().getStreamForFunction(global);

      mainStream.insert(mainStream.begin(), globalStream.begin(), globalStream.end());

    }
  }

  //error: more than one main (or no main at all)
  //will be catched by findEntryPoint
}

bool CodeEmitter::prepare()
{
  SymPtr sym = SymbolTable::GetSingleton().getGlobalNamespace();
  currentCodeOffset = 0;
  
  injectPrologEpilog(sym);
  insertGlobalInit(sym);

  //adesso calcoliamo la lunghezza di ogni funzione
  //e la scriviamo
  computeFunctionSize(sym);

  return true;
}

bool CodeEmitter::emitClass(SymPtr sym)
{
  ClassHeader ch;


  //trattiamo il puntatore come un ID
  ch.size = sizeof(ClassHeader) + 
            sym->vtable.getVirtualCount() * sizeof(int) + 
            sym->getFunctionCount() * sizeof(FunctionEntry) + 
            sym->getNativeFunctionCount() * sizeof(NativeFunction);
  currentFileOffset += ch.size;
  ch.tag = reinterpret_cast<tag_t>(sym);
  if (sym->isDerived)
    ch.parentTag = reinterpret_cast<tag_t>(sym->parent);
  else
    ch.parentTag = 0;

  ch.objectSize = sym->getObjectSize();
  ch.functionCount = sym->getFunctionCount();

  if (sym->isVptrPresent)
    ch.virtualCount = sym->vtable.getVirtualCount();
  else
    ch.virtualCount = 0;

  ch.nativeCount = sym->getNativeFunctionCount();

  fwrite(&ch, sizeof(ClassHeader), 1, f);
  //fflush(f);
  
  
  for (int i = 0; i < ch.virtualCount; ++i)
  {
    int funcIdx = sym->vtable.getEntry(i)->functionIndex;
    fwrite(&funcIdx, sizeof(int), 1, f);
  }

  FunctionEntry fe;
  
  std::list<SymPtr>::iterator it;
  for (it = sym->declList.begin(); it != sym->declList.end(); ++it)
  {
    if ((*it)->symKind == SYM_FUNCTION)
    {
      fe.ordinal = (*it)->functionIndex;
      fe.localCount = (*it)->getLocalCount();
      fe.paramCount = (*it)->getParamCount();
      fe.isNative = (*it)->isNative;
      if (!fe.isNative)
      {
        fe.startAddress = (*it)->functionOffset;
      }
      else
      {
        fe.nativeID = reinterpret_cast<tag_t>(*it);
      }
      
      fwrite(&fe, sizeof(FunctionEntry), 1, f);
    }
  }

  //emit native info
  NativeFunction ne;
  for (it = sym->declList.begin(); it != sym->declList.end(); ++it)
  {
    if ((*it)->symKind == SYM_FUNCTION && (*it)->isNative)
    {
      ne = SysImportTable::GetSingleton().get(*it);
      ne.ID = reinterpret_cast<tag_t>(*it);
      fwrite(&ne, sizeof(NativeFunction), 1, f);
    }
  }

  return true;
}

void CodeEmitter::emitClassRec(SymPtr sym)
{
  emitClass(sym);
  std::list<SymPtr>::iterator it;
  for (it = sym->declList.begin(); it != sym->declList.end(); ++it)
  {
    if ((*it)->symKind == SYM_CLASS)
    {
      emitClassRec(*it);
    }
  }  
}

bool CodeEmitter::emitSymCode(SymPtr sym)
{
  std::list<SymPtr>::iterator it;
  //dobbiamo solo visitare le funzioni (unici simboli che possono
  //contenere codice eseguibile) nello stesso ordine della
  //computeFunctionSize
  for (it = sym->declList.begin(); it != sym->declList.end(); ++it)
  {
    if ((*it)->symKind == SYM_FUNCTION)
    {
      //trasforma il codice nello stream inserendo i giusti riferimenti
      //per:

      //-salti
      IE.ICodeStream_resolveLabels(*it);      

      //-riferimenti a variabili
      // sostituiamo il riferimanto al SymPtr con l'ordinale 
      // calcolato da indexVariables
      std::list<ICodeNode>& codeStream = IE.getStreamForFunction(*it);
      std::list<ICodeNode>::iterator code_it;
      for (code_it = codeStream.begin(); code_it != codeStream.end(); ++code_it)
      {
        if (code_it->tag == INSTRUCTION)
        {
          code_it->instr.resolveVarOperands();
          code_it->instr.resolveTypeOperands();
        }
      }

      //scrivi lo stream su file
      for (code_it = codeStream.begin(); code_it != codeStream.end(); ++code_it)
      {
        if (code_it->tag == INSTRUCTION)
        {
          Instruction i = code_it->instr;
          fwrite(&i, sizeof(Instruction), 1, f);
          //fflush(f);
        }
      }
    }
    else if ((*it)->symKind == SYM_CLASS)
    {
      emitSymCode(*it);
    }
  }

  return true;
}

CodeEmitter CodeEmitterSingleton;

