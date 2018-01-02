#pragma once

#include <cmath>
#include <complex>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <thread>

//#define _CHECKBOUNDS_ 1
//#define _USESTDVECTOR_ 1
//#define _USENRERRORCLASS_ 1
//#define _TURNONFPES_ 1

// Macro-like inline functions

template <typename T> inline void SWAP(T &a, T &b) {
  T dum = a;
  a = b;
  b = dum;
}

template <typename T> inline T SQR(const T a) { return a * a; }

template <typename T, typename T1, typename T2>
inline const T &MAX(const T1 &a, const T2 &b) {
  return b > a ? (b) : (a);
}

template <typename T, typename T1, typename T2>
inline const T &MIN(const T1 &a, const T2 &b) {
  return b < a ? (b) : (a);
}

template <typename T, typename T1, typename T2>
inline T SIGN(const T1 &a, const T2 &b) {
  return b >= 0 ? (a >= 0 ? a : -a) : (a >= 0 ? -a : a);
}

  // Exception handling

  // Example:
  //
  //	try {
  //		somebadroutine();
  //	}
  //	catch(Error s) {Catch(s);}

#ifndef _USENRERRORCLASS_
#define throw(message){printf("ERROR: %s\n     in file %s at line %d\n",       \
                              message, __FILE__, __LINE__);                    \
                       throw(1); }
#else
struct Error {
  char *message;
  char *file;
  int line;
  Error(char *m, char *f, int l) : message(m), file(f), line(l) {}
};
#define throw(message) throw(Error(message, __FILE__, __LINE__));
void Catch(Error err) {
  printf("ERROR: %s\n     in file %s at line %d\n", err.message, err.file,
         err.line);
  exit(1);
}
#endif

// Real value types
typedef double real;
