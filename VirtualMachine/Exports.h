
#ifndef EXPORTS_H_INCLUDED
#define EXPORTS_H_INCLUDED

#include <string>
#include <iostream>

void print(const std::string& s)
{
  std::cout << s << std::endl;
}

#include <hash_map>
#include <stdio.h>

std::hash_map<std::string, FILE*> files;

void fprint(const std::string& filename, const std::string& s)
{
  if (files.find(filename) == files.end())
  {
    //FILE* f;
    files[filename] = fopen(filename.c_str(), "w");
    //++fileno;
  }
  fprintf(files[filename], "%s", s.c_str());
  fflush(files[filename]);
}

void fprintln(const std::string& filename, const std::string& s)
{
  if (files.find(filename) == files.end())
  {
    files[filename] = fopen(filename.c_str(), "w");
    //++fileno;
  }
  fprintf(files[filename], "%s\n", s.c_str());
  fflush(files[filename]);
}


#include <math.h>

//
//string.hs

std::string stradd(const std::string& a, const std::string& b)
{
  std::string s = a + b;
  return s;
}

//


#endif //EXPORTS_H_INCLUDED


