// well I think the idea, with NDEBUG is that with one flag we can remove all
// the asserts. 
#ifndef NO_DEBUG_H  // But maybe you still want asserts... 

#pragma once

#include <iostream>
#include <vector>

template<typename T>
std::ostream& operator<<(std::ostream& lhs, const std::vector<T>& rhs) {
  for (const auto& elem : rhs)
    std::cout << elem << ' ';
  std::cout << "\n";
}

#endif
