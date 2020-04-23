#include "csv_matrix.h"
#include "stopwatch.h"

#include <iostream>

int main(int argc, const char** argv) {
  Stopwatch t;
  t.start();
  CSVMatrix matrix("data/sample/groceryData.csv", true);
  std::cout << t.now() << "\n";
}