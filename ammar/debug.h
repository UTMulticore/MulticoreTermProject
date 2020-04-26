#ifndef NO_DEBUG_H 

#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>

template<typename T>
std::ostream& operator<<(std::ostream& lhs, const std::vector<T>& rhs) {
  for (const auto& elem : rhs)
    std::cout << elem << ' ';
  std::cout << "\n";
  return lhs;
}

template<typename K, typename V>
std::ostream& operator<<(std::ostream& lhs, 
                        const std::unordered_map<K, V>& rhs) {
  for (const auto& pair : rhs) {
    std::cout << pair.first << " : " << pair.second << "\n";
  }
  return lhs;
}

#endif
