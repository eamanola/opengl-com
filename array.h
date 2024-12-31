#ifndef ARRAY_H
#define ARRAY_H

#include <vector>

template <class T>
class Array
{
public:
  Array(const std::vector<T>& vector)
  :
  first(&vector[0]),
  length(vector.size())
  {
  }
  ~Array() {};

  const T& operator [] (unsigned int index) const { return first[index]; }
  const unsigned int size() const { return length; }

  private:
    const T* first;
    const unsigned int length;
};


#endif
