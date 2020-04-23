#pragma once 

#include <cassert>
#include <chrono>

/**
  Small Wrapper of C++ clock functionality. Intended to be used for basic profiling.
  
  TimeInterval is a POD which contains
*/

/** Calculate Potential overflow.



*/
struct TimeInterval {
  unsigned int seconds_;
  unsigned int milliseconds_;
  unsigned int microseconds_;
  unsigned int nanoseconds_;

  static double totalTime(const TimeInterval& t) {
    return t.seconds_ + t.milliseconds_ * 1e-3 + t.microseconds_ * 1e-6 + t.nanoseconds_ * 1e-9;
  }
};

struct Stopwatch {

  // void start(); // starts the timer or resets to 0.
  // double now(); // returns the curent duration since Timer started.


  double end(); // stops the timer and returns the ending time
  double duration(); // returns the duration of the 

  // Why am I using CLOCK_PROCESS_CPUTIME_ID ? 
  // because this timer will often be used to profile functions. And in a multiprocess
  // environment I dont care, how long it takes for it to get the cpu. I only care
  // about how efficiently it runs on the cpu with its alotted time.


  Stopwatch(){}; // how to init ?

  void start() {
    start_time_ = std::chrono::steady_clock::now();
  }

  int now() {
    auto now = std::chrono::steady_clock::now();
    auto diff = now - start_time_;
    return diff.count(); // returns the tick differnece
  }

 private:

  std::chrono::time_point<std::chrono::steady_clock> start_time_;
  std::chrono::time_point<std::chrono::steady_clock> stop_time_;
};