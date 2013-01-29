

#ifndef OUTPUT_MANAGER_H_INCLUDED_
#define OUTPUT_MANAGER_H_INCLUDED_

#include <Singleton.h>
#include <fstream>
#include <string>

class OutputManager : public Singleton<OutputManager>
{
public:

  enum output_type
  {
    ERROR = 0,
    WARNING,
    INFO,
    NOTHING
  };

private:
  std::fstream output;
  void open(const std::string& filename);

  OutputManager(const OutputManager&);
  OutputManager& operator=(const OutputManager&);

public:

  OutputManager() { open("build.log"); }
  OutputManager(const std::string& filename) { open(filename); } 

  void out(output_type, const char* string, ...);
  void out(const std::string& s);
};

#endif //OUTPUT_MANAGER_H_INCLUDED_


