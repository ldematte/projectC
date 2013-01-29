
#define SINGLETON_IMPLEMENTATION
#include "Singleton.h"
#include "VirtualMachine.h"
#include "CodeLoader.h"
#include "RTSysExportTable.h"
#include "Exports.h"

#ifdef WIN32
#include "PEFile.h"
#endif


//#define 
//macro->tabella funzioni disponibili
//a run time, caricamento/confronto


int main(int argc, char* argv[])
{
  if (argc <= 1)
  {
    std::cout << "Usage: vm executable" << std::endl;
    return -1;
  }

  EXPORT(print);
  EXPORT(fprintln);
  EXPORT(fprint);
  EXPORT(cosf);
  EXPORT(stradd);

  /*
  PEFile pe;
  HMODULE module = pe.openFile("msvcrt.dll");

  DWORD dwRet = GetLastError();
  pe.importBindings(module);
  */

  VirtualMachine::GetSingleton().eip = 0;

  CodeLoader loader;



  if (!loader.load(argv[1]))
  {
    std::cout << "Error opening file \"" << argv[1] 
              <<"\": the file does not exixst or is not a valid VM executable" << std::endl;
    return -1;
  }

  VirtualMachine::GetSingleton().setBaseAddress(loader.getAddress());
  VirtualMachine::GetSingleton().setStackSize(loader.getStackSize());
  
  VirtualMachine::GetSingleton().reserveGlobal();
  VirtualMachine::GetSingleton().setEntryPoint();

  VirtualMachine::GetSingleton().instrStreamSize = loader.getStreamSize();

  VirtualMachine::GetSingleton().dumpDASM("dump.txt");
  VirtualMachine::GetSingleton().exec();

  return 0;
}


