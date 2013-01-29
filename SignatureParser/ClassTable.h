
#ifndef CLASS_TABLE_H_INCLUDED_
#define CLASS_TABLE_H_INCLUDED_

#include "Types.h"
#include "../Singleton.h"

#include <string>
#include <hash_map>

class ClassTable : public Singleton<ClassTable>
{
public:
  ClassType& get(const std::string& className)
  {
    return classMap[className];
  }

  void addSubType(const std::string& className, TypePtr type)
  {
    ClassType& ct = get(className);
    ct->typeList.push_back(type);
  }


private:

  std::hash_map<std::string, ClassType> classMap;
  //std::vector<std::string> tokens;

};

#endif //CLASS_TABLE_H_INCLUDED_



