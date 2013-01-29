
#ifndef SINGLETON_H_INCLUDED_
#define SINGLETON_H_INCLUDED_

/* Copyright (C) Scott Bilas, 2000. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Scott Bilas, 2000"
 */

#include "common.h"

template <typename T> class Singleton
{
    static T* ms_Singleton;

public:
    Singleton( void )
    {
        assert( !ms_Singleton );
        ptrdiff_t offset = (ptrdiff_t)(T*)1 - (ptrdiff_t)(Singleton <T>*)(T*)1;
        ms_Singleton = (T*)((ptrdiff_t)this + offset);
    }
   ~Singleton( void )
        {  assert( ms_Singleton );  ms_Singleton = 0;  }
    static T& GetSingleton( void )
        {  assert( ms_Singleton );  return ( *ms_Singleton );  }
    static T* GetSingletonPtr( void )
        {  return ( ms_Singleton );  }
};

template <typename T> T* Singleton <T>::ms_Singleton = 0;


#endif //SINGLETON_H_INCLUDED_



