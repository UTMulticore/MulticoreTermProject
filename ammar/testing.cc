#include "csv_matrix.h"
// #define NO_DEBUG_H 
#include "debug.h"
#include "fp_growth.h"
#include "stopwatch.h"

#include <iostream>
#include <string>

#include <map>


int main(int argc, const char** argv) {
  Stopwatch t;
  t.start();

  const char* path = "data/small/groceryData.csv";
  bool forced_load = true;
  bool columns_present = true;

  try {

    CSVMatrix<std::string> data_set(path, columns_present, forced_load); 
    data_set.removeColumn(0);
    //data_set.dumpMatrix();

    // Two-Pass over data:
    // First pass, calculate the support of each item
    // Second pass, sort based on support
    int min_support = 300;
    std::cout << "min_support:" <<  min_support << "\n";
    // Build the FP Tree
    FPGrowth fp(data_set, min_support);
    // Query FP Tree -> Find a way to display the results
    auto res = fp.mine();

    std::cout << "RESULT:\n";
    for (const auto& v : res) {
      for (const auto& item : v) {
        std::cout << item << " ";
      }
      std::cout << "\n";
    }
  } 
  
  catch(std::exception& e) {
    std::cout << e.what() << "\n";
  }

  std::cout << t.now() << "milliseconds \n";
}
