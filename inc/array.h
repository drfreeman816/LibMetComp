#pragma once

#include "metcomp.h"

/******************/
/*    1D Array    */
/******************/

template <typename T, size_t N> class Array {

  size_t _size;
  T *_ptr;

public:
  // Make type available
  typedef T type;
  // Zero array constructor
  explicit Array();
  // Initialize to constant value
  Array(const T &);
};

// Zero array constructor
template <typename T, size_t N>
Array<T, N>::Array() : _size(N), _ptr(new T[N]) {}

// Initialize to constant value
template <typename T, size_t N>
Array<T, N>::Array(const T &val) : _size(N), _ptr(new T[N]) {
#pragma omp for
  for (size_t i = 0; i < N; i++)
    _ptr[i] = val;
}
