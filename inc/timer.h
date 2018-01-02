#pragma once

#include "metcomp.h"
#include <chrono>

/***************/
/*    Timer    */
/***************/

class Timer {

  typedef std::chrono::time_point<std::chrono::high_resolution_clock> hdtime;
  typedef std::chrono::duration<real> duration;

  hdtime _start;
  hdtime _end;
  duration _elapsed;
  real _value;

public:
  // Constructors
  Timer(void);
  Timer(hdtime);
  // Start/Stop
  void start(void);
  void stop(void);
  // Return elapsed time
  // duration value(void);
  real val(void);
  // Destructor
  //~Timer(void);
};
