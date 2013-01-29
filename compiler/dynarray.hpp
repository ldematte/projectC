
#ifndef DYNARRAY_H_INCLUDED_
#define DYNARRAY_H_INCLUDED_

#include <vector>

//4k
const int chunck_size = 4096;

struct chunck
{
  char buffer[chunck_size];
  int bytes_left;

  chunck() { bytes_left = chunck_size; }
};

class dynarray
{
private:
  //int actual_size;  
  chunck first_chunck;
  int actual_chunck;
  //int actual_chunck_left;

  std::vector<chunck> chunck_vector;

public:

  dynarray() : chunck_vector(1)
  {
    actual_chunck = 0;
  }

  char at(ptrdiff_t i)
  {
    size_t chunck_no  = i / chunck_size;
    size_t chunck_idx = i % chunck_size;

    return chunck_vector[chunck_no].buffer[chunck_idx];
  }

  void insert(int i)
  {
    insert((char*)&i, sizeof(int));
  }

  void insert(const void* d, int dim)
  {
    char* data = (char*)d;
    int diff = dim;
    while ((chunck_vector[actual_chunck].bytes_left - diff) < 0)
    {
      diff = diff - chunck_vector[actual_chunck].bytes_left;
      memcpy(&(chunck_vector[actual_chunck].buffer[chunck_size - chunck_vector[actual_chunck].bytes_left]), data, chunck_vector[actual_chunck].bytes_left);
      data += chunck_vector[actual_chunck].bytes_left;
      chunck_vector[actual_chunck].bytes_left = 0;
      chunck_vector.push_back(chunck());
      ++actual_chunck;
      //chunck_vector[actual_chunck].chunck_left = chunck_size;      
    }

    memcpy(&(chunck_vector[actual_chunck].buffer[chunck_size - chunck_vector[actual_chunck].bytes_left]), data, diff);
    chunck_vector[actual_chunck].bytes_left -= diff;
  }

  void dump(FILE* f)
  {
    std::vector<chunck>::iterator i;
    for (i = chunck_vector.begin(); i != chunck_vector.end(); ++i)
    {
      fwrite(&(i->buffer[0]), (chunck_size - i->bytes_left), 1, f);
    }
  }

  size_t size()
  {
    return (chunck_vector.size() * chunck_size - chunck_vector[actual_chunck].bytes_left);
  }
};


#endif //DYNARRAY_H_INCLUDED_


