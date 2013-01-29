
#ifndef FUNCTION_TABLE_H_INCLUDED_
#define FUNCTION_TABLE_H_INCLUDED_

#include "../common.h"
#include "../Singleton.h"
#include "../CodeHeader.h"

#include <vector>
#include <hash_map>

/*
struct ClassHeader
{
  int size;
  tag_t tag;
  tag_t parentTag; //0 for nothing
  int objectSize;
  int functionCount;
  int virtualCount;
  //int[] vtable;
  //FunctionEntry[] funcTable;
};

struct FunctionEntry
{
  int ordinal;
  int localCount;
  int paramCount;
  int startAddress;

  //int FrameSize = ParamCount size + localCount + 1 (return point) [+ 1 this param];
};
*/

class ClassEntry : public ClassHeader
{
public:
  std::vector<int> vtable;
  std::hash_map<int, FunctionEntry> funcTable;
};

class ClassTable : public Singleton<ClassTable>
{
public:
  void insert(tag_t i, const ClassEntry& cls)
  {
    table[i] = cls;
  }

  ClassEntry& getClass(tag_t i)
  {
    return table[i];
  }

  int getEntryPoint()
  {
    return entryPoint;
  }

  void setEntryPoint(int i)
  {
    entryPoint = i;
  }

  int getGlobalSize()
  {
    return globalSize;
  }

  void setGlobalSize(int i)
  {
    globalSize = i;
  }


  tag_t getGlobalTag()
  {
    return globalTag;
  }

  void setGlobalTag(tag_t tag)
  {
    globalTag = tag;
  }

  bool isDowncastPossible(tag_t to, tag_t original)
  {
    ClassMap::iterator it;  

    if (original == to)
      return true;

    for (;;)
    {
      it = table.find(original);
      if (it == table.end())
      {
        //raise error (critical)
        //tag not in ClassTable
      }
      
      if (it->second.parentTag == 0)
        return false;
      if (it->second.parentTag == to)
        return true;

      original = it->second.parentTag;
    }
  }

private:

  typedef std::hash_map<tag_t, ClassEntry> ClassMap;
  ClassMap table;
  int entryPoint;
  int globalSize;
  tag_t globalTag;

};


#endif //FUNCTION_TABLE_H_INCLUDED_


