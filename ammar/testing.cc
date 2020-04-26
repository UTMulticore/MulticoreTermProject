#include "csv_matrix.h"
// #define NO_DEBUG_H 
#include "debug.h"
#include "stopwatch.h"

#include <iostream>
#include <string>
#include <unordered_map>

int main(int argc, const char** argv) {
  Stopwatch<std::chrono::microseconds> t;
  t.start();
  bool forced_load = true;

  try {
    CSVMatrix<double> matrix("data/sample/groceryData.csv", forced_load);
    //matrix.dumpMatrix();

    // Two-Pass over data:
    // First pass, calculate the support of each item
    // Second pass, sort based on support
    

    // Lol this is such a complicated way to do this, just manually type it.
    std::unordered_map<decltype(matrix)::matrix_type, double> supportMap; // Use a concurrent map when doing parallel


    // First row is column names...
    for (std::size_t r=1; r<matrix.getRows(); ++r) {
      for (std::size_t c=0; c<matrix.getCols(); ++c) {
        supportMap[matrix[r][c]] += 1;
      }
    }


    // could do this in conjucncition with the step above, just have to be aware of
    // floating point errors.  
    for (auto& pair : supportMap) { 
      pair.second = pair.second / matrix.getRows();
    }

    std::cout << supportMap;


    // Sort rows,



    // Build the FP Tree


    // Query FP Tree

  } 
  catch(std::exception& e) {
    std::cout << e.what() << "\n";
  }


  std::cout << t.now() << "ms \n";
}