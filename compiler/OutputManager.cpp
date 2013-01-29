

#include "OutputManager.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#ifndef _MSC_VER
#define _vsnprintf vsnprintf
#endif

//gcc 2.9x bug
#if defined(__GNUG__) 
  #if (__GNUC__ <= 2) && (__GCC_MINOR__ <= 9)
    #define ios_base ios
  #endif
#endif

#include "SourceManager.h"

void OutputManager::out(output_type ot, const char* string, ...)
{
  char buffer[1024];

  va_list	ap;
	va_start(ap, string);
  _vsnprintf(buffer, sizeof(buffer), string, ap);
	va_end(ap);   
   
  output << SourceManager::GetSingleton().getCurrentFile() << " (" << SourceManager::GetSingleton().getCurrentLineNumber() << ") : ";
  std::cout << SourceManager::GetSingleton().getCurrentFile() << " (" << SourceManager::GetSingleton().getCurrentLineNumber() << ") : ";


  switch (ot)
  {
  case ERROR: 
    {
      output << "error : ";
      std::cout << "error : ";
      SourceManager::GetSingleton().incrErrorCount();
    }
    break;
    
  case WARNING: 
    {
      output << "warning : ";
      std::cout << "warning : ";
      SourceManager::GetSingleton().incrWarningCount();
    }
    break;
  case INFO: 
    output << "info : ";
    std::cout << "info : ";
    break;
  default:
    break;
  }
  
  output << buffer << std::endl;
  std::cout << buffer << std::endl;
}

void OutputManager::out(const std::string& s)
{
  output << s << std::endl;
  std::cout << s << std::endl;
}

void OutputManager::open(const std::string& filename)
{
  output.open(filename.c_str(), std::ios_base::out | std::ios_base::trunc );
}

//The singleton instance
OutputManager TheOutputManger;

