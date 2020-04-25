#include "csv_matrix.h"
#include "debug.h"
#include "stopwatch.h"

#include <iostream>
#include <string>

int main(int argc, const char** argv) {
  Stopwatch t;
  t.start();
  bool forced_load = true;

  try {
    CSVMatrix<std::string> matrix("data/sample/small.csv", forced_load);
    matrix.dumpMatrix();

  } 
  catch(std::exception& e) {
    std::cout << e.what() << "\n";
  }

  // Build the FP-Tree.

  std::cout << t.now() << "\n";
  std::cout << "Done!\n";
}