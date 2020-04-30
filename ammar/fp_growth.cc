
#include "debug.h"
#include "fp_growth.h"
#include "fp_tree.h"

#include <algorithm>
#include <omp.h>

static bool isData(const std::string& str) {
  std::cout << "Got the read\n";
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
  omp_lock_t lock;
  omp_init_lock(&lock);
  #pragma omp parallel for num_threads(num_threads_)
  for (std::size_t i=0; i<data_set.getRows(); i++) {
    auto item_set = filterLowSupport(data_set.sample(i));
    std::sort(item_set.begin(), item_set.end(), [&](std::string& a, std::string& b){
      return support_map[a] > support_map[b];
    });
    // lock
    omp_set_lock(&lock);
    tree_.add(item_set);
    omp_unset_lock(&lock);
    // unlock
  }
  //TODO: sortMapByValue(tree_.item_lists_);
}


void FPGrowth::initSupportMap(const CSVMatrix<std::string>& data_set) {
  std::cout << "here\n";
  omp_lock_t lock;
  omp_init_lock(&lock);
 // std::cout << "REAL Lock shit done!\n";
  #pragma omp parallel for num_threads(num_threads_) collapse(2)
    for (std::size_t r=0; r<data_set.getRows(); ++r) {
      for (std::size_t c=0; c<data_set.getCols(); ++c) {
       // std::cout << "HERE I AM!\n";
        if (isData(data_set[r][c])) {
          omp_set_lock(&lock);
          support_map[data_set[r][c]] += 1;
          omp_unset_lock(&lock);
        }
      }
    }

  std::cout << "supp map built!\n";
}

FPGrowth::FPGrowth(const CSVMatrix<std::string>& data_set, std::uint32_t min_supp, int num_threads=1) 
                  : min_support_(min_supp), num_threads_(num_threads) {
  
    initSupportMap(data_set);
    //std::cout << support_map;
    std::cout << "building tree\n";
    buildTree(data_set);
    std::cout << "tree built\n";
    //tree_.printTree();
    //std::cout << "\n";
}

std::vector<std::vector<std::string>> FPGrowth::mine() {
  return tree_.mine(min_support_, num_threads_);
}



