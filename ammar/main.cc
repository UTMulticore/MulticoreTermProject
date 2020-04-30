#include "csv_matrix.h"
// #define NO_DEBUG_H 
#include "debug.h"
#include "fp_growth.h"
#include "stopwatch.h"

#include <iostream>
#include <fstream>
#include <string>


void writeToFile(std::vector<std::vector<std::string>>& results);

int main(int argc, const char** argv) {
  // params
  const char* path = "data/small/groceryData.csv";
  bool forced_load = true;
  bool columns_present = true;
  int min_support = 30;

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

  auto done = stopwatch.now();

  // std::cout << "RESULT:\n";
  // for (const auto& v : res) {
  //   for (const auto& item : v) {
  //     std::cout << item << " ";
  //   }
  //   std::cout << "\n";
  // }
  
  writeToFile(res);
  std::cout << done << " milliseconds \n";
}


void writeToFile(std::vector<std::vector<std::string>>& results) {
  std::ofstream file("output.txt");
  assert(file.is_open());
  for (const auto& v : results) {
    for (const auto& item : v) {
      file << item << ' ';
    }
    file << '\n';
  }
}