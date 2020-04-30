
#include "debug.h"
#include "fp_growth.h"
#include "fp_tree.h"

#include <algorithm>

static bool isData(const std::string& str) {
  if (str.size() == 0)
    return false;
  for (auto c : str) {
    if (std::isalnum(c))
      return true;
  }
  return false;
}

std::vector<std::string> FPGrowth::filterLowSupport(std::vector<std::string> v) {
  std::vector<std::string> ret;
  for (const auto& item : v) {
    if (support_map.count(item) && support_map[item] >= min_support_)
      ret.push_back(item);
  }
  return ret;
}


// when we build the tree, we need to filter out items of a low support and sort
// based on support
void FPGrowth::buildTree(const CSVMatrix<std::string>& data_set) {
  for (std::size_t i=0; i<data_set.getRows(); i++) {
    auto unfiltered = data_set.sample(i);
    auto item_set = filterLowSupport(unfiltered);
    std::sort(item_set.begin(), item_set.end(), [&](std::string& a, std::string& b){
      return support_map[a] > support_map[b];
    });
    tree_.add(item_set);
  }
  //TODO: sortMapByValue(tree_.item_lists_);
  //tree_.printTree();
  //std::cout << "\n";
}


void FPGrowth::initSupportMap(const CSVMatrix<std::string>& data_set) {
    for (std::size_t r=0; r<data_set.getRows(); ++r) {
      for (std::size_t c=0; c<data_set.getCols(); ++c) {
        if (isData(data_set[r][c]))
          support_map[data_set[r][c]] += 1;
      }
    }
}

FPGrowth::FPGrowth(const CSVMatrix<std::string>& data_set, std::uint32_t min_supp) 
                  : min_support_(min_supp) {
  
    initSupportMap(data_set);
    buildTree(data_set);
}

std::vector<std::vector<std::string>> FPGrowth::mine() {
  return tree_.mine(min_support_);
}



