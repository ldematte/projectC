
#ifndef NATIVE_FUNCTION_H_INCLUDED
#define NATIVE_FUNCTION_H_INCLUDED


#include <vector>
#include <string>

#include "common.h"
#include "constants.h"

// possible calling conventions
enum CallType
{
  CALL_CDECL, 
  CALL_STDCALL, 
  CALL_THISCALL,
  CALL_THISCALLVARARG,
  CALL_FASTCALL
};


// function specification
struct NativeFunction
{

  void*        proc;

  char name[256];
  tag_t ID;
  tag_t returnType;
  
  CallType callType;

  //tag_t* paramTypes;
};


#endif //NATIVE_FUNCTION_H_INCLUDED





