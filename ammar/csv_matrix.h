// RENAME FILE 
// This file holds everything to do with data i/o.

// Both of my datasets will be csv files. So I will be desigining these
// two classes first.


// IDEAS: 
// This first class design will be pull everything into memory. 
// The second design will pull from memory as needed
// The third will be combination of the two, having only parts of it in memory.

#pragma once 

#include <cstddef>
#include <fstream>
#include <vector>


// Turn this into a template class so that CSVMatrix can
// be of any type
class CSVMatrix {

 private:  
  std::ifstream file_stream_;     // just for system agnostic.  
  std::vector<std::vector<std::string>> data_matrix_;
  
  unsigned int num_rows_;    // Could potentially be dealing with some large datasets thus be wary of overflow
  unsigned int num_columns_; 
  
  std::size_t file_byte_size_;
  std::string file_name_;

  std::vector<std::string> col_names_;

 public:
  // By default, lazy initialization of Matrix.
  CSVMatrix(const char* file_path, bool forced_load=false);

  // Maybe an LRU for loading in huge datasets ? Kind've like paging and swapping
  unsigned int getRows() const {return num_rows_;}
  unsigned int getCols() const {return num_columns_;}

  // Returns a random row from the 
  std::vector<std::string> sample();
  std::vector<std::string> sample(std::size_t idx);

  // modifying values in csv matrix
  template<class UnaryFunc>
  void mapAll(UnaryFunc f);

  
  // Should also aim to provvide easy access of Matrix.
};