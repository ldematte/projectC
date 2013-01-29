
#include "CodeLoader.h"
#include <stdio.h>
#include <malloc.h>

#include "ClassTable.h"
#include "RTStringTable.h"
#include "RTSysExportTable.h"
#include "../NativeFunction.h"


bool CodeLoader::load(char* fileName)
{  
  FILE* f = fopen(fileName, "rb");
  if (!f)
    return false;

  fread(&h, sizeof(Header), 1, f);

  if (h.magic != 0x00415246)
    return false;

  size_t stringTableSize;
  fread(&stringTableSize, sizeof(size_t), 1, f);

  size_t tableRead = 0;

  ClassTable::GetSingleton().setEntryPoint(h.entryPointIndex);
  ClassTable::GetSingleton().setGlobalTag(h.globalTag);
  ClassTable::GetSingleton().setGlobalSize(h.globalDataSize);

  while (tableRead < stringTableSize)
  {
    int strLen;

    char* buffer;

    fread(&strLen, sizeof(int), 1, f);
    tableRead += sizeof(int);
    buffer = new char[strLen + 1];
    fread(buffer, strLen * sizeof(char), 1, f);
    buffer[strLen] = 0;
    tableRead += strLen * sizeof(char);
    
    RTStringTable::GetSingleton().table.push_back(buffer);
  }

  //now reads the code
  int codeSize = h.objectsOffset - h.textOffset;
  codeStream = (Instruction*)malloc(codeSize);
  fread(codeStream, codeSize, 1, f);

  //now the ClassTable
  int bytesRead = 0;
  int tableSize = h.fileSize - h.objectsOffset;
  while (bytesRead < tableSize)
  {
    ClassEntry ce;
    fread(&ce, sizeof(ClassHeader), 1, f);

    int i;
    for (i = 0; i < ce.virtualCount; ++i)
    {
      int idx;
      fread(&idx, sizeof(int), 1, f);
      ce.vtable.push_back(idx);
    }

    for (i = 0; i < ce.functionCount; ++i)
    {
      FunctionEntry fe;
      fread(&fe, sizeof(FunctionEntry), 1, f);
      ce.funcTable[fe.ordinal] = fe;
    }

    for (i = 0; i < ce.nativeCount; ++i)
    {
      NativeFunction ne;
      fread(&ne, sizeof(NativeFunction), 1, f);
      RTSysExportTable::GetSingleton().match(ne, ne.ID);
    }


    bytesRead = bytesRead + sizeof(ClassHeader) + 
      ce.virtualCount * sizeof(int) +
      ce.functionCount * sizeof(FunctionEntry) + 
      ce.nativeCount * sizeof(NativeFunction);

    ClassTable::GetSingleton().insert(ce.tag, ce);
  }
  
  return true;
}

void CodeLoader::unload()
{
  free(codeStream);
}

CodeLoader CodeLoaderSingleton;


