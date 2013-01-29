
#ifndef SOURCE_MANAGER_H_INCLUDED_
#define SOURCE_MANAGER_H_INCLUDED_

#include <Singleton.h>
#include <string>
#include <stack>

extern char* yytext;

class SourceManager : public Singleton<SourceManager>
{
private:

  int line_number;
  int error_count;
  int warn_count;

  std::string current_file;

public:
  SourceManager()
    : line_number(0), error_count(0), warn_count(0) { }

  void incrementLineNumber() { ++line_number; }
  int getCurrentLineNumber() { return line_number; } 
  void setCurrentLineNumber(int line) { line_number = line; } 
 
  std::string getCurrentToken() 
  { 
    return std::string(yytext); 
  }
  std::string getCurrentFile() 
  { 
    return current_file; 
  } 

  void setCurrentFile(const std::string& s) 
  { 
    current_file = s;  
  }
  
  int checkType(int& key);

  std::string getIncludeFile(const std::string& filename);

  void incrErrorCount() { ++error_count; }
  void incrWarningCount() { ++warn_count; }
  int getErrorCount() { return error_count; }
  int getWarningCount() { return warn_count; }
};


#endif //SOURCE_MANAGER_H_INCLUDED_

