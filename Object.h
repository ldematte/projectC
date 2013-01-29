
#ifndef OBJECT_H_INCLUDED_
#define OBJECT_H_INCLUDED_

#include "constants.h"
#include <string>

class ObjectData
{
public:

  void addRef()
  {
    ++refCount;
  };

  void release()
  {
    if (--refCount == 0)
      delete this;
  };

  int refCount;
  char* data;  
};

class Object
{
public:
  tag_t actualID;
  tag_t originalID;

  //un qualcosa (ID, puntatore...) che ci dica che tipo e'
  //ci servira' per il cast e le chiamate virtuali
  virtual void addRef()  { assert(data); data->addRef();  }
  virtual void release() { assert(data); data->release(); }

  virtual char* getData() { return data->data; }
  ObjectData* data;

  virtual ~Object() { };
};

class StringObject : public Object
{
public:
  int refCount;
  std::string stringData;

  void addRef()
  {
    ++refCount;
  };

  void release()
  {
    --refCount;
  };

  char* getData() 
  {
    return (char*)(&stringData);
  };
};

class ArrayObject : public Object
{
public:
  int dimension;
  int refCount;

  char* arrayData;

  void addRef()
  {
    ++refCount;
  };

  void release()
  {
    if (--refCount == 0)
      if (arrayData)
      {
        delete arrayData;
        arrayData = NULL;
      }
  };

  char* getData() 
  {
    return (arrayData);
  };
};

#endif //OBJECT_H_INCLUDED_

