#include "csv_matrix.h"
// #define NO_DEBUG_H 
#include "debug.h"
#include "fp_growth.h"
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

  const char* path = "data/sample/smallGrocery.csv";
  bool forced_load = true;
  bool columns_present = true;

  try {

    CSVMatrix<std::string> data_set(path, columns_present, forced_load); 
    data_set.removeColumn(0);
    data_set.dumpMatrix();

    // Two-Pass over data:
    // First pass, calculate the support of each item
    // Second pass, sort based on support
    
    std::unordered_map<std::string, int> support_map; // Use a concurrent map when doing parallel

    // First row is column names...
    for (std::size_t r=0; r<data_set.getRows(); ++r) {
      for (std::size_t c=0; c<data_set.getCols(); ++c) {
        if (isData(data_set[r][c]))
          support_map[data_set[r][c]] += 1;
      }
    }

    std::cout << "\n";
    std::cout << support_map << "\n";


    // Sort rows
    for (std::size_t r=0; r<data_set.getRows(); ++r) {
      std::sort(data_set[r], data_set[r] + data_set.getCols(), 
                    [&](const std::string& a, const std::string& b){ 
                      return support_map[a] > support_map[b]; });
      }

    data_set.dumpMatrix();

    // Build the FP Tree
    FPGrowth fp(data_set);


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