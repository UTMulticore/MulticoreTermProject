#include "csv_matrix.h"
// #define NO_DEBUG_H 
#include "debug.h"
#include "fp_growth.h"
#include "stopwatch.h"

#include <iostream>
#include <string>

#include <map>


int main(int argc, const char** argv) {
  // params
  const char* path = "data/small/groceryData.csv";
  bool forced_load = true;
  bool columns_present = true;
  int min_support = 20;

  int num_threads = std::stoi(argv[1]);

  Stopwatch stopwatch;
  stopwatch.start();
  
  // Load Data
  CSVMatrix<std::string> data_set(path, columns_present, forced_load); 
  data_set.removeColumn(0);
  std::cout << "done!\n";

  // Build the FP Tree
  FPGrowth fp(data_set, min_support, num_threads);
  std::cout << "time to mine!\n";
  // Mine FP Tree -> Find a way to display the results
  auto res = fp.mine();

  std::cout << "RESULT:\n";
  for (const auto& v : res) {
    for (const auto& item : v) {
      std::cout << item << " ";
    }
    std::cout << "\n";
  }
  

  std::cout << stopwatch.now() << "milliseconds \n";
}
