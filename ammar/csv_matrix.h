// This file holds everything to do with data i/o.

// Both of my datasets will be csv files. So I will be desigining these
// two classes first.

// Author: Ammar Sheikh


// IDEAS: 
// This first design will be pull everything into memory. 
// The second design will pull from memory as needed
// The third will be combination of the two, having only parts of it in memory.

#pragma once 

#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <system_error>
#include <vector>

#include <sys/types.h>
#include <sys/stat.h>


/*
  TODO: Extra Line In CSV Failing!
        What are the limitations of this class ?
         - overflow?, underflow ?
        Exceptions ?

        I don't need to store the num_rows, and num_columns
        if I am storing like that. 
        Because, then I can just query, std::vector sizes.


        Fix Style !!!!! <------- Function names are mixed case.

*/


template<class T>
class CSVMatrix {

 private:  
  std::ifstream file_stream_;     // system agnostic.  
  std::vector<T> data_matrix_;    // convenience, but don't need size/cap 
  
  std::size_t num_rows_;   
  std::size_t num_columns_;
  
  std::size_t file_byte_size_;
  std::string file_name_;
  std::vector<std::string> col_names_;

  bool is_loaded_;

  void InitMatrix();

 public:
  // By default, lazy initialization of Matrix.
  CSVMatrix(const char* file_path, bool forced_load=false);
  ~CSVMatrix();

  CSVMatrix& operator=(const CSVMatrix&) = delete;


  // Maybe an LRU for loading in huge datasets ? Kind've like paging and swapping

  // ACCESSORS
  std::size_t getRows() const noexcept { return num_rows_; }
  std::size_t getCols() const noexcept { return num_columns_; }

  // Returns a copy of a row in Matrix
  std::vector<T> sample();
  std::vector<T> sample(std::size_t idx);

  // Applies value to all elements in Matrix
  template<class UnaryFunc>
  void mapAll(UnaryFunc f);

  // Applies value to all rows in Matrix.


  // TODO; REMOVE!
  void dumpMatrix();
};


// What should happen to these ?
static std::size_t GetFileSize(const char* path) {
  struct stat ret;
  int rc = stat(path, &ret);
  return rc == 0 ? ret.st_size : 0;  
}
 
static void LoadCSVLine(std::string& csv_line, 
                        std::vector<std::string>& matrix_row) {
  std::size_t position = 0;
  std::size_t end_position = 0;

  while(end_position != std::string::npos) {
    end_position = csv_line.find(',', position);
    matrix_row.push_back(csv_line.substr(position, end_position-position));
    position = end_position + 1;
  }
}

template<class T>
CSVMatrix<T>::CSVMatrix(const char* file_path, bool forced_load) 
                    : file_name_(file_path), file_stream_(file_path), 
                    num_rows_(0), num_columns_(0), is_loaded_(false) {
  
  assert(file_stream_.is_open());
  if (!file_stream_.is_open())
    throw std::filesystem::filesystem_error("file failed to open", 
                                            std::error_code()); 

  file_byte_size_ = GetFileSize(file_path);

  if (!forced_load) 
    return;

  InitMatrix();
  // init rows, and columns
  is_loaded_ = true;
}

template<class T>
CSVMatrix<T>::~CSVMatrix() {
  file_stream_.close();
}

// Loads the entire CSV file into the data_matrix_.
template<class T>
void CSVMatrix<T>::InitMatrix() {
  assert(num_rows_ == 0 && num_columns_ == 0);

  std::size_t buffer_size = 500; // <---- This had to be tweaked. How do I choose this in the future when I don't know how long a line may be ?
  std::string buffer;
  buffer.resize(buffer_size);

  while (!file_stream_.eof()) {
    std::vector<std::string> row;
    buffer.replace(0, buffer_size, buffer_size, '\0');
    file_stream_.getline(&buffer[0], buffer_size);
    LoadCSVLine(buffer, row);
    data_matrix_.push_back(std::move(row));
  }
}

template<class T>
std::vector<T> CSVMatrix<T>::sample() {
  if (!is_loaded_) 
    InitMatrix();

  assert(num_rows_ > 0);
  return data_matrix_[rand() % num_rows_];
}

template<class T>
std::vector<T> CSVMatrix<T>::sample(std::size_t idx) {
  assert(num_rows_ > 0);
  assert(idx < num_rows_);
  if (!is_loaded_) 
    InitMatrix();
  return data_matrix_[idx];
}

template<class T>
void CSVMatrix<T>::dumpMatrix() {
  std::cout << "DIMS: " << num_rows_ << " " << num_columns_ << "\n";
  for (auto& row : data_matrix_) {
    for (auto& elem : row) {
      std::cout << elem << " ";
    }
    std::cout << "\n";
  } 
}