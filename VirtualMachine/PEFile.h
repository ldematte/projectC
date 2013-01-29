

#ifndef PEFILE_H_INCLUDED_
#define PEFILE_H_INCLUDED_

#include <string>
#include <iostream>

#include <windows.h>
#include <winnt.h>


#include "../common.h"

class PEFile
{
public:
  HMODULE openFile(const std::string& filename);

  bool importBindings(HMODULE module);


};


#endif //PEFILE_H_INCLUDED_

