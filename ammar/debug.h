#ifndef NO_DEBUG_H 

#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>

// These are useful for quick debugging.

template<typename T>
std::ostream& operator<<(std::ostream& lhs, const std::vector<T>& rhs) {
  for (const auto& elem : rhs)
    lhs << elem << ' ';
  lhs << "\n";
  return lhs;
}

template<typename K, typename V>
std::ostream& operator<<(std::ostream& lhs, 
                        const std::unordered_map<K, V>& rhs) {
  for (const auto& pair : rhs) {
    lhs << pair.first << " : " << pair.second << "\n";
  }
  return lhs;
}

#endif
