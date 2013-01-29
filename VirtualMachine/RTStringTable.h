
#ifndef RT_STRING_TABLE_H_INCLUDED_
#define RT_STRING_TABLE_H_INCLUDED_

#include "common.h"
#include <vector>
#include <string>
#include "../Singleton.h"

class RTStringTable : public Singleton<RTStringTable>
{
public:

  std::vector<std::string> table;

  std::string& getString(int idx)
  {
    return table[idx];
  }

  int getNewEmptyString()
  {
    table.push_back(std::string());
    return (int)(table.size() - 1);
  }

  void setString(int idx, const std::string& s)
  {
    table[idx] = s;
  }

  //TODO: migliorare la gestione, magari con un
  //meccanismo di rilasco e lista di nuovi interi
  //riusabili
};

#endif //RT_STRING_TABLE_H_INCLUDED_

