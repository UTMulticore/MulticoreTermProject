//
//  csv_matrix.h
// 
// Author: Ammar Sheikh

#pragma once 

#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <system_error>
#include <type_traits>
#include <vector>

#include <sys/types.h>
#include <sys/stat.h>

#include "debug.h"

/*
  TODO: Extra Line In CSV Failing!
        What are the limitations of this class ?
         - overflow?, underflow ?
        Exceptions ?

        Fix Style !!!!! <------- Function names are mixed case.


        Its not easy to construct objects of a type that you don't know.
        What if you don't know how to construct the object ???? I'm confusing myself here.
        For now lets just assume its std::string, std::int, or std::double


        How to handle huge datasets ? Maybe some sort of swapping ?
        with LRU ?

        Need to document

*/


template<class T>
class CSVMatrix {

  static_assert(std::is_same<T, std::string>::value 
                || std::is_same<T, int>::value
                || std::is_same<T, double>::value, 
                "Currently Unsupported type." 
                "Try string, double, or int");

 private:  

  std::ifstream file_stream_;     // system agnostic.  
  std::vector<T> data_matrix_;    // convenience, but don't need size/cap 

  std::string file_name_;
  std::size_t file_byte_size_;    // This is not really being used, Consider removing

  std::vector<std::string> col_names_;
  std::size_t num_rows_;   
  std::size_t num_columns_;
  
  bool is_loaded_;

  
  void InitMatrix(bool col_names_present);
  
 public:

  typedef T matrix_type;


  // By default, lazy initialization of Matrix.
  CSVMatrix(const char* file_path, bool col_names ,bool forced_load=false);
  ~CSVMatrix();

  CSVMatrix& operator=(const CSVMatrix&) = delete;

  std::size_t getRows() const noexcept { return num_rows_; }
  std::size_t getCols() const noexcept { return num_columns_; }

  // Return a pointer to start of row.
  T* operator[](std::size_t row){
    assert(is_loaded_);
    assert(row >=0 && row < num_rows_);
    return &data_matrix_[row*num_columns_]; 
  }

  std::vector<T> sample();
  std::vector<T> sample(std::size_t idx) const;


  void removeColumn(std::size_t col_idx);

  // Applies value to all elements in Matrix
  template<class UnaryFunc>
  void mapAll(UnaryFunc f);

  //TODO: Applies value to all rows in Matrix.

  // Temp debugging...
  void dumpMatrix();
};


// Move this somewehere else once project ends.
static std::size_t GetFileSize(const char* path) {
  struct stat ret;
  int rc = stat(path, &ret);
  return rc == 0 ? ret.st_size : 0;  
}


// TODO: LOOK INTO THIS
//
// Not to happy with how I am currently function overloading. Also I don't
// think a substr is needed for the double and integer overload.
//
// I could potentially combine them into one template function. 
// So for instance, if the user provides a function to transform
// the csv file data into an object of type T 
//
// Also, I am using the attributes to get the damn warnings to shutup.
// but is there any other benefit ?
//
// Also, need to fix bytes_read error, where values do not exist.
//

[[maybe_unused]] static void LoadCSVLine(std::string& csv_line, 
                        std::vector<std::string>& data, std::size_t bytes_read) {
  std::size_t position = 0;
  std::size_t end_position = 0;
  while(end_position != std::string::npos) {
    end_position = csv_line.find(',', position);
    if (end_position == std::string::npos) // <------------------------------ This a bad temporary solution. Fix this for all 3.
      data.push_back(csv_line.substr(position, bytes_read-position));
    else
      data.push_back(csv_line.substr(position, end_position-position));
    position = end_position + 1;
  }
}

[[maybe_unused]] static void LoadCSVLine(std::string& csv_line, 
                        std::vector<int>& data, std::size_t bytes_read) {
  std::size_t position = 0;
  std::size_t end_position = 0;

  while(end_position != std::string::npos) {
    end_position = csv_line.find(',', position);
    data.push_back(std::stoi(csv_line.substr(position, end_position-position)));
    position = end_position + 1;
  }
}


[[maybe_unused]] static void LoadCSVLine(std::string& csv_line, 
                        std::vector<double>& data, std::size_t bytes_read) {
  std::size_t position = 0;
  std::size_t end_position = 0;

  while(end_position != std::string::npos) {
    end_position = csv_line.find(',', position);
    data.push_back(std::stod(csv_line.substr(position, end_position-position)));
    position = end_position + 1;
  }
}



template<class T>
CSVMatrix<T>::CSVMatrix(const char* file_path, bool col_names, 
                      bool forced_load) : file_stream_(file_path), 
                      file_name_(file_path), num_rows_(0), 
                      num_columns_(0), is_loaded_(false) {
  
  assert(file_stream_.is_open());
  if (!file_stream_.is_open())
    throw std::filesystem::filesystem_error("file failed to open",  // Hmmm, should I, or leabe exceptions out?
                                            std::error_code()); 
  file_byte_size_ = GetFileSize(file_path);
  if (!forced_load) 
    return;
  InitMatrix(col_names);
  is_loaded_ = true;
}

template<class T>
CSVMatrix<T>::~CSVMatrix() {
  file_stream_.close();
}

// Loads the entire CSV file into the data_matrix_.
template<class T>
void CSVMatrix<T>::InitMatrix(bool col_names) {
  assert(num_rows_ == 0 && num_columns_ == 0);
  std::size_t buffer_size = 500; // <----- This had to be tweaked. How do I choose this in the future when I don't know how long a line may be ?
  std::string buffer;
  buffer.resize(buffer_size);

  if (col_names) {
    file_stream_.getline(&buffer[0], buffer_size);
    LoadCSVLine(buffer, col_names_, file_stream_.gcount());
    buffer.replace(0, buffer_size, buffer_size, '\0');
  }


  int rows = 0;
  while (!file_stream_.eof()) {
    file_stream_.getline(&buffer[0], buffer_size);
    std::size_t bytes_read = file_stream_.gcount();
    if (bytes_read  <= 1)
      break;
    LoadCSVLine(buffer, data_matrix_, bytes_read);
    buffer.replace(0, buffer_size, buffer_size, '\0');
    ++rows;
  }
  num_rows_ = rows;
  //std::cout << data_matrix_.size();
  std::cout << rows << "\n"; 
  assert(data_matrix_.size() % num_rows_ == 0);
  num_columns_ = data_matrix_.size() / num_rows_;
}

// TODO; Remove both of these sample functions, or rewrite,they are part of old impl 
template<class T>
std::vector<T> CSVMatrix<T>::sample() {
  if (!is_loaded_) {
    InitMatrix(false);
    is_loaded_ = true;
  }
  assert(num_rows_ > 0);
  return data_matrix_[rand() % num_rows_];
}

template<class T>
std::vector<T> CSVMatrix<T>::sample(std::size_t idx) const {
  assert(num_rows_ > 0);
  assert(idx < num_rows_);
  // if (!is_loaded_) { 
  //   InitMatrix(false);
  //   is_loaded_ = true;
  // }
  assert(num_rows_ > 0);
  auto begin = data_matrix_.begin() + (idx * num_columns_);
  auto end = begin + num_columns_;
  return std::vector<T>(begin, end);
}

template<class T>
void CSVMatrix<T>::dumpMatrix() {
  std::cout << "DIMS: " << num_rows_ << " " << num_columns_ << "\n";
  std::cout << "COLUMN NAMES: " << col_names_ << "\n";
  for (std::size_t i=0; i<num_rows_; i++) {
    for (std::size_t j=0; j<num_columns_; j++) {
      std::cout << data_matrix_[i*num_columns_ + j] << "|";
    }
    std::cout << "\n";
  }
}


template<class T>
void CSVMatrix<T>::removeColumn(std::size_t col_idx) {
  assert(is_loaded_);
  assert(col_idx >= 0 && col_idx < num_columns_);
  
  auto col_key = col_idx % num_columns_;
  std::vector<T> dest_matrix;
  // Preallocate. dest_matrix.
  for (int i=0; i<data_matrix_.size(); i++) {
    if (i % num_columns_ != col_key) {
      dest_matrix.push_back(data_matrix_[i]);
    }
  }

  data_matrix_ = std::move(dest_matrix);
  num_columns_--;
  assert(data_matrix_.size() == num_columns_ * num_rows_);
}