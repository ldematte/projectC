
#ifndef CODE_HEADER_H_INCLUDED_
#define CODE_HEADER_H_INCLUDED_

#include "constants.h"

struct Header
{
  int magic;
  short versionMajor;
  short versionMinor;

  int stackSize;  
  int globalDataSize;
  int textSize;
  
  int entryPointIndex;
  tag_t globalTag;
  
  int textOffset;
  int objectsOffset;
  int fileSize;

  int CRC; //unused, must be Zero
};

struct ClassHeader
{
  int size;
  tag_t tag;
  tag_t parentTag; //0 for nothing
  int objectSize;
  int functionCount;
  int virtualCount;
  int nativeCount;
  //int[] vtable;
  //FunctionEntry[] funcTable;
};

struct FunctionEntry
{
  int ordinal;
  int localCount;
  int paramCount;
  union
  {
    int startAddress;
    tag_t nativeID;
  };
  bool isNative;

  //int FrameSize = ParamCount size + localCount + 1 (return point) [+ 1 this param];
};

#endif //CODE_HEADER_H_INCLUDED_

