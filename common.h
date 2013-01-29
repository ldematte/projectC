
#ifndef DEMA_COMMON_H_INCLUDED_
#define DEMA_COMMON_H_INCLUDED_


/*
#include <windows.h>
#include <winnt.h>
*/

#if (defined(_MSC_VER) && _MSC_VER < 1300)

#pragma warning (disable: 4786)

//unreferenced inline function has been removed
#pragma warning (disable: 4514)
#pragma warning (disable: 4127)

//for scope trick (VC++ 6.0 and below)
#define for if (0) ; else for

#endif //(_MSC_VER < 1300)


#include <assert.h>

#define ASSERT assert
#ifdef DEBUG

#define VERIFY(x) assert(x); x

#define Q(string) # string
#define PRINT(x) std::cout << Q(x : ) << x << std::endl;



#else //DEBUG
#define VERIFY(x) x
#endif //DEBUG

//#define HAS_WAVE_PREPROCESSOR

//Alcuni warning sono superflui
//vengono disabilitati per compilare in modo pulito con livello
//di warning 4


#if (defined(_MSC_VER))

//parametro formale senza riferimenti
#pragma warning (disable: 4100)

//using std instead of stdext for hash_map
#pragma warning(disable : 4996)

//32-bit -> 64-bit: perche' sarebbe male? non perdo nulla
#pragma warning(disable : 4312)

#else

//stupid GCC
#include <string>
#include <hash_map> 

 /** BEGIN FIX **/ 
 namespace std 
 { 
   template<> struct hash< std::string > 
   { 
     size_t operator()( const std::string& x ) const 
     { 
       return hash< const char* >()( x.c_str() ); 
     } 
   }; 
 }
 /** END FIX **/ 

#endif //msc


#ifdef WIN32

//this function checks to see if a pointer is pointing to “bad food”. 
//Debug versions of memory managers typically use magic numbers like these for 
//pre- and post-fill on memory blocks.
inline bool IsMemoryBadFood(unsigned int d)
{
  return ( ( d == 0xDDDDDDDD )     // crt: dead land (deleted objects)
        || ( d == 0xCDCDCDCD )     // crt: clean land (new, uninit’d objects)
        || ( d == 0xFDFDFDFD )     // crt: no man's land (off the end)
        || ( d == 0xCCCCCCCC )     // vc++: stack objects init’d with this
        || ( d == 0xFEEEFEEE )     // ? nt internal ?
        || ( d == 0xBAADF00D ) );  // winnt: nt internal "not yours" filler
}


#endif //WIN32


typedef unsigned char byte;



//a nice helper to zero something out
template <typename T> 
inline void ZeroObject( T& object )
{  
  memset( &object, 0, sizeof( T ) );  
}


template <typename T>
void range(T min, T& num, T max)
{
  assert(min < max);
  if (num < min)
    num = min;
  else if (num > max)
    num = max;
}

template<class Itor> 
inline Itor prior( Itor it ) 
{
  return --it; 
}

template<class Itor> 
inline Itor next( Itor it ) 
{   
  return ++it; 
}


#endif //DEMA_COMMON_H_INCLUDED_

