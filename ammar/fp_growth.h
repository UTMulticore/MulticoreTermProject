
#pragma once

#include "csv_matrix.h"
#include "fp_tree.h"

class FPGrowth {


  FP_Tree tree_;

  void buildTree(const CSVMatrix<std::string>&);

 public:

  FPGrowth(const CSVMatrix<std::string>& data_set);


};