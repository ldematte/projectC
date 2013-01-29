
#ifndef STRING_TABLE_H_INCLUDED_
#define STRING_TABLE_H_INCLUDED_

#include <string>
#include <map>
#include <vector>

#include <Singleton.h>

//valid indexes are starts from 1

class StringTable : public Singleton<StringTable>
{
private:
  std::map<std::string, int> table;
  int nextIndex;

public:

  std::vector<std::string> Table;

  std::string& getString(int idx)
  {
    return Table[idx];
  }

  StringTable()
  {
    nextIndex = 0;
    Table.push_back("__DUMMY__");
  }

  int insert(std::string s)
  {
    if (s.at(0) == '\"')
      s = s.substr(1, s.length() - 2);

    std::map<std::string, int>::iterator it = table.find(s);
    
    if (it == table.end())
    {
      table[s] = ++nextIndex;
      Table.push_back(s);
      return nextIndex;
    }
    else
    {
      return it->second;
    }     
  }

  int search(std::string s)
  {
    std::map<std::string, int>::iterator it = table.find(s);
    
    if (it == table.end())
      return 0;
    else
      return it->second;
  }
};

#endif //STRING_TABLE_H_INCLUDED_


