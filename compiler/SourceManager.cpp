

#include "SourceManager.h"
#include "type.h"
#include "SyntaxTree.h"
#include "minicpp.cpp.h"
#include "SymbolTable.h"
#include "ErrorManager.h"

int SourceManager::checkType(int& key)
{
  key = SymbolTable::GetSingleton().setLastToken(yytext);
    
  if(SymbolTable::GetSingleton().lookupTypeSymbol(yytext))
    return TYPENAME;
  else
    return IDENTIFIER;
}

std::string SourceManager::getIncludeFile(const std::string& filename)
{
  size_t index = filename.find ("#include");
  if (index == std::string::npos)
  {
    //raise error
    std::string s = "Cannot open include file \"" + filename + "\"";
    ErrorManager::GetSingleton().logAndRaise(s.c_str());
  }

  //trim
  size_t idx1 = filename.find_first_not_of(" \t\"", 8);
  size_t idx2 = filename.find_last_not_of(" \t\"");
  return filename.substr(idx1, idx2 - idx1 - 1);
}




SourceManager SourceManagerSingleton;


