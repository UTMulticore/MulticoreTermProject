#pragma once 

#include <cassert>
#include <chrono>

/**
  Small Wrapper of C++ clock functionality. Intended to be used for basic profiling.
  
  // I don't think this is working
*/

template<class Duration = std::chrono::milliseconds>
struct Stopwatch {

  Stopwatch() = default;

  void start() {
    start_time_ = std::chrono::steady_clock::now();
  }

  // To store as integer or double ?
  uint64_t now() {
    auto now = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<Duration>(start_time_ - now).count();
  }

  // stops the timer and returns the ending time
  uint64_t stop() {
    stop_time_ = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<Duration>(start_time_ - stop_time_).count();
  }


 private:

  std::chrono::steady_clock::time_point start_time_;
  std::chrono::steady_clock::time_point stop_time_;
};