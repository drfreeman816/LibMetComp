#pragma once

//#define _CHECKBOUNDS_ 1
//#define _USESTDVECTOR_ 1
//#define _USENRERRORCLASS_ 1
//#define _TURNONFPES_ 1

// NR3 recommended headers
#include <cmath>
#include <complex>
#include <ctype.h>
#include <fcntl.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <vector>
// Some other headers
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iterator>
#include <map>
#include <string>
#include <utility>

using namespace std;

// Vector class

#ifdef _USESTDVECTOR_
#define Vector vector
#else

template <typename T>
class Vector{

  int _size;  // Size of vector
  T *_ptr;

public:

  // Constructors
  Vector();
  Vector(const Vector& rhs);                    // Copy constructor
  explicit Vector(size_t n);                    // Zero vector
  Vector(size_t n, const T& val);               // Initialize with constant value
  Vector(size_t n, const T& v1, const T& v2);   // Initialize as a linspace
  Vector(size_t n, const T& val);               // Initialize to array

  // Make type available
  typedef T type;

  // Assignment
  Vector& operator=(const Vector& rhs);

  // Access element by index

};
