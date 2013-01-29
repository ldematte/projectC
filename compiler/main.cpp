
#include <stdio.h>
#include <string>
#include <hash_map>
#include <iostream>


#include "type.h"
#include "SyntaxTree.h"
#include "minicpp.cpp.h"
#include "SymbolTable.h"
#include "ScopeManager.h"
#include "SourceManager.h"
#include "OutputManager.h"
#include "ICodeEmitter.h"
#include "CodeEmitter.h"
#include "lexer.h"
#include "Preprocessor.h"

/*

  rimane da fare:
  -scrittura su file (con la 

  soprattutto: quando vengono calcolati gli offset delle varibili
  dalla classe - funzione (cioe', base dello stack 
  ebp in questo caso)? a me servono i primi gia' a tempo di generazione
  dell'I-code!! ora vengono calcolati DOPO la generazione dell'I-code,
  cosa sufficiente per gli offset di varibili locali-parametri dalla
  base dello stack/activation record in EBP ma non per le classi
  oggetti: quando accedo ai membri, devo gia' sapere l'offset.

  quindi, devo fare una passata dopo aver costruito l'intera symbol
  table. Oppure, dare alle variabili gli offset gia' al tempo di 
  insermento nella ST, dovrebbe essere fattibile.

  poi, rimane comunque la fase di passata per eliminare i reference
  a symbol nell'I-code con indici assoluti o relativi nello stack

  rimane anche la faccenda delle conversioni di tipo, la mergeType
  in particolare. specialmente con gli UDT (le classi) e in presenza
  di ereditarieta'.

  Nella VM le parti piu' spinose sono l'utilizzo di pointer
  in ogni operazione (come negli ADD e SUB), devono essere fatte in 
  modo da fare il "dereferencig"

  */


class CmdLine
{
private:

  char** Args;
  int size;

  struct param
  {
    int idx;
    bool flag;
  };

  std::hash_map< std::string, param > optionMap;
  typedef std::hash_map< std::string, param >::iterator opIter;

  bool hasFileArg(int index)
  {
    if (index >= size - 1)
      return false;

    if (Args[index + 1][0] == '-')
      return false;
    
    return true;
  }

public:

  CmdLine(char* argv[], int n)
  {
    Args = argv;
    size = n;

    for (int i = 1; i < n; ++i)
    {
      param p;
      p.idx = i;
      p.flag = (argv[i][0] == '-');
      optionMap[argv[i]] = p;
    }

    if (Args[1][0] == '-')
      validF = false;
    else
      fileName = Args[1];


    assemblyF = hasArg("-a", assemblyName);
    outputF = hasArg("-o", outputName);

    verboseF = hasArg("-v");
    helpF = hasArg("-h");
    stF = hasArg("-s");
    optimizeF = hasArg("-O");
    treeF = hasArg("-t");
  }

  bool hasArg(const char* name, std::string& filename)
  {
    bool ret = false;
    opIter it;
    it = optionMap.find(name);
    if (it != optionMap.end())
    {
      int index = (*it).second.idx;
      if (hasFileArg(index))
      {
        ret = true;
        filename = Args[index + 1];
      }  
    }

    return ret;
  }

  bool hasArg(const char* name)
  {
    bool ret = false;
    opIter it;
    it = optionMap.find(name);
    if (it != optionMap.end())
    {
      ret = true;
    }
    return ret;
  }


  bool assemblyF;
  std::string assemblyName;

  bool outputF;
  std::string outputName;

  bool validF;
  std::string fileName;

  bool verboseF;
  bool helpF;
  bool stF;
  bool optimizeF;
  bool treeF;
};

void printH()
{
  std::cout << "SMC 32-bit compiler for XASM (VM)" << std::endl;
  std::cout << "(C)Lorenzo Dematte' 2003 - 2004" << std::endl;
  std::cout << std::endl;
}

void printUsage()
{
  printH();

  std::cout << "usage: smc inputfile [options]" << std::endl;
  std::cout << "options: -a assemblyFileName output assembly listing to assemblyFileName" << std::endl;
  std::cout << "         -o outputFileName output machine code to outputFileName" << std::endl;
  std::cout << "         -v dispaly (very) verbose output" << std::endl;
  std::cout << "         -O perform some optimizations" << std::endl;
  std::cout << "         -t dispaly parse tree" << std::endl;
  std::cout << "         -s dispaly Symbol Table" << std::endl;
  std::cout << std::endl;
}

int main (int argc, char* argv[])
{

  if (argc <= 1)
  {
    //error
    printUsage();
    return 0;
  }

  CmdLine cmdline(argv, argc); 

  if (!cmdline.validF || cmdline.helpF)
  {
    printUsage();
    return 0;
  }

  //do some init
  std::string preprocessedFile = preprocess(cmdline.fileName);
  lexer::openFile(preprocessedFile);

  SymbolTable::GetSingleton().init();
  ScopeManager::GetSingleton().init();
  
  if (cmdline.verboseF)
    yydebug = 1;
  else
    yydebug = 0;

  try
  {

    if (yyparse() != 0)
    {
      //error
      OutputManager::GetSingleton().out(OutputManager::ERROR, "Parse error");
    }
    SymbolTable::GetSingleton().clearTempTokens();
    //after the parse, we need the scope manager again
    ScopeManager::GetSingleton().init();

    SyntaxTree::GetSingleton().checkType();
    SyntaxTree::GetSingleton().generateCode();

    if (cmdline.treeF)
      SyntaxTree::GetSingleton().dump();

    if (cmdline.stF)
      SymbolTable::GetSingleton().dump();
   
  }
  catch(CompilerException* ce)
  {
    //do nothing
    OutputManager::GetSingleton().out(ce->errorMessage);
  }
  
  
  OutputManager::GetSingleton().out("\n");
  OutputManager::GetSingleton().out(OutputManager::NOTHING, " %d error(s), %d warning(s)", 
    SourceManager::GetSingleton().getErrorCount(), 
    SourceManager::GetSingleton().getWarningCount());
  OutputManager::GetSingleton().out("\n");

  if (SourceManager::GetSingleton().getErrorCount() == 0)
  {
    if (cmdline.optimizeF)
      IE.optimize();

    if (cmdline.assemblyF)
    {
      FILE* af = fopen(cmdline.assemblyName.c_str(), "w");
      if (!af)
      {
        std::cout << "Cannot open \"" << (cmdline.assemblyName) << "\" for XASM output" <<  std::endl;
      }
      else
      {        
        std::cout << "Emitting XASM stream..." << std::endl;
        IE.dump(af);
      }
    }

    if (!cmdline.outputF)
    {
      std::string name;
      size_t index = cmdline.fileName.find_last_of('.');

      if (index != std::string::npos)
        name = cmdline.fileName.substr(0, index);
      cmdline.outputName = name + ".dat";
    }
    
    if (!CodeEmitter::GetSingleton().createOutput(cmdline.outputName))
    {
      std::cout << "Cannot open \"" << cmdline.outputName << "\" for code output" <<  std::endl;
      return -1;
    }

    std::cout << "Emitting VM code stream..." << std::endl;


    CodeEmitter::GetSingleton().emitHeader();
    CodeEmitter::GetSingleton().emitStringTable();
    CodeEmitter::GetSingleton().emitCode();
    CodeEmitter::GetSingleton().emitObjectTable();
    CodeEmitter::GetSingleton().emitHeader();

    std::cout << "Done!" << std::endl;

    return 0;
  }
  else
    return -1;
}

