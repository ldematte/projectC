
#include "Preprocessor.h"
#include "common.h"

#ifdef HAS_WAVE_PREPROCESSOR

#include <boost/config.hpp>

#include <wave/cpp_context.hpp>
#include <fstream>
#include <iostream>
#include <string>

std::string preprocess(const std::string& file)
{
  // The following preprocesses a given input file.
  // Open the file and read it into a string variable
  std::ifstream instream(file);    
  std::string input(std::istreambuf_iterator<char>(instream.rdbuf());
  std::istreambuf_iterator<char>());

  // The template wave::cpplexer::lex_token<> is the token 
  // type to be used by the Wave library.
  // This token type is one of the central types throughout 
  // the library, because it is a template parameter to many 
  // of the public classes and templates and it is returned 
  // from the iterators itself.
  typedef wave::context< std::string::iterator, wave::cpplexer::lex_token<> > context_t;

  // The C++ preprocessor iterators shouldn't be constructed 
  // directly. These are to be generated through a 
  // wave::context<> object. Additionally this wave::context<> 
  // object is to be used to initialize and define different 
  // parameters of the actual preprocessing.
  context_t ctx(input.begin(), input.end(), "input.cpp");
  context_t::iterator_t first = ctx.begin();
  context_t::iterator_t last = ctx.end();

  // The preprocessing of the input stream is done on the fly 
  // behind the scenes during the iteration over the 
  // context_t::iterator_t based stream. 
  while (first != last) 
  {
    std::cout << (*first).get_value();
    ++first;
  }

  //modifica il nome
   return file;
}

#else //HAS_WAVE_PREPROCESSOR

std::string preprocess(const std::string& file)
{
  return file;
}

#endif //HAS_WAVE_PREPROCESSOR

