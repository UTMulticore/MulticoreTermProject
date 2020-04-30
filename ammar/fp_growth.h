
#pragma once

#include "csv_matrix.h"
#include "fp_tree.h"

#include <atomic>
#include <unordered_map>
#include <vector>

class FPGrowth {


  FP_Tree tree_;
  std::unordered_map<std::string, int> support_map;
  std::uint32_t min_support_;
  int num_threads_;

  void buildTree(const CSVMatrix<std::string>&);
  void initSupportMap(const CSVMatrix<std::string>&);
  std::vector<std::string> filterLowSupport(std::vector<std::string>);

 public:

  FPGrowth(const CSVMatrix<std::string>& data_set, std::uint32_t min_support, int num_threads);

  void run();
  std::vector<std::vector<std::string>> mine();

};