

#ifndef NATIVE_CALL_H_INCLUDED
#define NATIVE_CALL_H_INCLUDED

typedef unsigned int DWORD;

DWORD Call_cdecl( const void* args, size_t sz, DWORD func );
DWORD Call_stdcall( const void* args, size_t sz, DWORD func );
DWORD Call_thiscall( const void* args, size_t sz,
                    void* object, DWORD func );
DWORD Call_thiscallvararg( const void* args, size_t sz,
                          void* object, DWORD func );


inline __declspec ( naked ) DWORD GetST0( void )
{
  DWORD f;                // temp var
  __asm
  {
    fstp dword ptr [f]      // pop ST0 into f
    mov eax, dword ptr [f]  // copy into eax
    ret                     // done
  }
}

inline __declspec ( naked ) DWORD GetEIP( void )
{
  __asm
  {
    mov eax, dword ptr [esp]
    ret
  }
}


#endif //NATIVE_CALL_H_INCLUDED

