
#ifndef MY_STACK_H_INCLUDED_
#define MY_STACK_H_INCLUDED_

#include <vector>

template <class T>
class MyStack : public std::vector<T>
{
public:
  
  void push(const T& x) { push_back(x); } 
  T& top() { return back(); }
  void pop() { pop_back(); }
};


#endif //MY_STACK_H_INCLUDED_

