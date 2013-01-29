
#ifndef ERROR_MANAGER_H_INCLUDED_
#define ERROR_MANAGER_H_INCLUDED_

#include "../Singleton.h"
#include "OutputManager.h"

#include <string>
#include <exception>

class CompilerException : public std::exception
{
public:
  CompilerException(const char* msg)
  {
    errorMessage = msg;
  }

  std::string errorMessage;
};

class ErrorManager : public Singleton<ErrorManager>
{
public:
  void logAndRaise(const char* message)
  {
    OutputManager::GetSingleton().out(OutputManager::ERROR, message);
    raise(message);
  }

  void raise(const char* msg)
  {
    throw (new CompilerException(msg));
  }
};

#endif //ERROR_MANAGER_H_INCLUDED_


