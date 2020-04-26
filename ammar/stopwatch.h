#pragma once 

#include <cassert>
#include <chrono>


// Small Wrapper of C++ clock functionality. Intended to be used for basic profiling.
//
// Not working, need to look up docs for std::chrono
//
// Need to test Stopwatch.
// Author: Ammar Sheikh


template<class Duration = std::chrono::milliseconds>
struct Stopwatch {

  Stopwatch() = default;

  void start() {
    start_time_ = std::chrono::steady_clock::now();
  }

  uint64_t now() {
    auto now = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<Duration>(start_time_ - now).count();
  }

  uint64_t stop() {
    stop_time_ = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<Duration>(start_time_ - stop_time_).count();
  }

 private:

  std::chrono::steady_clock::time_point start_time_;
  std::chrono::steady_clock::time_point stop_time_;
};