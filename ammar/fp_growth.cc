
#include "debug.h"
#include "fp_growth.h"


/*
  Fuck the path bullshit, why don't you just input it as a vector
  It would simplfy the whole process.
*/
void FPGrowth::buildTree(const CSVMatrix<std::string>& data_set) {
  for (std::size_t i=0; i<data_set.getRows(); i++) {
    auto sample = data_set.sample(i);
    tree_.add(sample);
    std::cout << "size: " << sample.size() << "vec: " << sample << "\n";
  }
  tree_.printTree();
}

FPGrowth::FPGrowth(const CSVMatrix<std::string>& data_set) {
  buildTree(data_set);
}