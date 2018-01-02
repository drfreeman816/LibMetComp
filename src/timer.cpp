#include "timer.h"

/***************/
/*    Timer    */
/***************/

// Constructors
Timer::Timer(void) : _start(std::chrono::high_resolution_clock::now()) {}
Timer::Timer(hdtime t) : _start(t) {}

// Start/Stop
void Timer::start(void) { _start = std::chrono::high_resolution_clock::now(); }

void Timer::stop(void) {
  _end = std::chrono::high_resolution_clock::now();
  _elapsed = _end - _start;
  _value = _elapsed.count();
}

// Return elapsed time
real Timer::val(void) { return _value; }
