#include "csv_matrix.h"
// #define NO_DEBUG_H 
#include "debug.h"
#include "stopwatch.h"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>
#include <unordered_map>


bool isData(const std::string& str);

int main(int argc, const char** argv) {
  Stopwatch t;
  t.start();

  bool forced_load = true;

  try {
    CSVMatrix<std::string> matrix("data/sample/small.csv", forced_load);

  
    // Two-Pass over data:
    // First pass, calculate the support of each item
    // Second pass, sort based on support
    
    std::unordered_map<decltype(matrix)::matrix_type, double> supportMap; // Use a concurrent map when doing parallel

    auto cardinalty = matrix.getRows() - 1;
    // First row is column names...
    for (std::size_t r=1; r<matrix.getRows(); ++r) {
      for (std::size_t c=0; c<matrix.getCols(); ++c) {
        if (c != 0 && isData(matrix[r][c])){
          supportMap[matrix[r][c]] += 1.0 / cardinalty; // be aware of floating point errors when we do it like this
        }
      }
    }

    std::cout << supportMap;


    // Sort rows
    for (std::size_t r=1; r<matrix.getRows(); ++r) {
      std::sort(matrix[r] + 1, matrix[r] + matrix.getCols(), 
                    [&](const std::string& a, const std::string& b){ 
                      return supportMap[a] > supportMap[b]; });
      }
  
    // Build the FP Tree


    // Query FP Tree -> Find a way to display the results

  } 
  
  catch(std::exception& e) {
    std::cout << e.what() << "\n";
  }


  std::cout << t.now() << "milliseconds \n";
}


bool isData(const std::string& str) {
  if (str.size() == 0)
    return false;
  for (auto c : str) {
    if (std::isalnum(c))
      return true;
  }
  return false;
}