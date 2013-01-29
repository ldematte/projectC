
#include "NativeCall.h"

DWORD Call_cdecl( const void* args, size_t sz, DWORD func )
{
  DWORD rc;               // here's our return value...
  __asm
  {
    mov   ecx, sz       // get size of buffer
    mov   esi, args     // get buffer
    sub   esp, ecx      // allocate stack space
    mov   edi, esp      // start of destination stack frame
    shr   ecx, 2        // make it dwords
    rep   movsd         // copy params to real stack
    call  [func]        // call the function
    mov   rc,  eax      // save the return value
    add   esp, sz       // restore the stack pointer
  }

  return ( rc );
}


DWORD Call_stdcall( const void* args, size_t sz, DWORD func )
{
  DWORD rc;               // here's our return value...
  __asm
  {
    mov   ecx, sz       // get size of buffer
      mov   esi, args     // get buffer
      sub   esp, ecx      // allocate stack space
      mov   edi, esp      // start of destination stack frame
      shr   ecx, 2        // make it dwords
      rep   movsd         // copy it
      call  [func]        // call the function
      mov   rc,  eax      // save the return value
  }

  return ( rc );
}

DWORD Call_thiscall( const void* args, size_t sz,
                    void* object, DWORD func )
{
  DWORD rc;               // here's our return value...
  _asm
  {
    mov   ecx, sz       // get size of buffer
      mov   esi, args     // get buffer
      sub   esp, ecx      // allocate stack space
      mov   edi, esp      // start of destination stack frame
      shr   ecx, 2        // make it dwords
      rep   movsd         // copy it
      mov   ecx, object   // set "this"
      call  [func]        // call the function

      mov   rc,  eax      // save the return value
  }
  return ( rc );
}

DWORD Call_thiscallvararg( const void* args, size_t sz,
                          void* object, DWORD func )
{
  DWORD rc;               // here's our return value...
  _asm
  {
    mov   ecx, sz       // get size of buffer
      mov   esi, args     // get buffer
      sub   esp, ecx      // allocate stack space
      mov   edi, esp      // start of destination stack frame
      shr   ecx, 2        // make it dwords
      rep   movsd         // copy it
      mov   ecx, object   // set "this"
      push  ecx           // push it on the stack
      call  [func]        // call the function

      mov   rc,  eax      // save the return value
        mov   eax, sz       // ready to restore stack pointer
        add   eax, 4        // pushed ecx too
        add   esp, eax      // restore the stack pointer
  }
  return ( rc );
}



/*
void SetupFunctionExports( void )
{
  {
    Function function;

    function.m_Name       = "Baz";
    function.m_Proc       = Baz;
    function.m_ReturnType = VAR_FLOAT;
    function.m_ParamTypes . push_back( VAR_INT );
    function.m_ParamTypes . push_back( VAR_STRING );
    function.m_CallType   = CALL_CDECL;

    g_Functions.push_back( function );
  }

  {
    Function function;

    function.m_Name       = "Tel";
    function.m_Proc       = Tel;
    function.m_ReturnType = VAR_INT;
    function.m_CallType   = CALL_CDECL;

    g_Functions.push_back( function );
  }
}
*/

