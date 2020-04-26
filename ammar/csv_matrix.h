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


/*
  TODO: Extra Line In CSV Failing!
        What are the limitations of this class ?
         - overflow?, underflow ?
        Exceptions ?

        I don't need to store the num_rows, and num_columns
        if I am storing like that. 
        Because, then I can just query, std::vector sizes.


        Fix Style !!!!! <------- Function names are mixed case.


        Its not easy to construct objects of a type that you don't know.
        What if you don't know how to construct the object ???? I'm confusing myself here.


        For now lets just assume its std::string, std::int, or std::double



        How to handle huge datasets ? Maybe some sort of swapping ?
        with LRU ?

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
  std::size_t file_byte_size_;

  std::vector<std::string> col_names_;
  std::size_t num_rows_;   
  std::size_t num_columns_;
  
  bool is_loaded_;

  
  void InitMatrix();
  
 public:

  typedef T matrix_type;


  // By default, lazy initialization of Matrix.
  CSVMatrix(const char* file_path, bool forced_load=false);
  ~CSVMatrix();

  CSVMatrix& operator=(const CSVMatrix&) = delete;

  // ACCESSORS
  std::size_t getRows() const noexcept { return num_rows_; }
  std::size_t getCols() const noexcept { return num_columns_; }

  // Return a pointer to start of row.
  T* operator[](std::size_t row){
    assert(is_loaded_);
    assert(row >=0 && row < num_rows_);
    return &data_matrix_[row*num_columns_]; 
  }

  std::vector<T> sample();
  std::vector<T> sample(std::size_t idx);

  // Applies value to all elements in Matrix
  template<class UnaryFunc>
  void mapAll(UnaryFunc f);

  // Applies value to all rows in Matrix.

  void dumpMatrix();
};


// Move this somewehere else on project end.
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

[[maybe_unused]] static void LoadCSVLine(std::string& csv_line, 
                        std::vector<std::string>& data, std::size_t bytes_read) {
  //std::cout << "bytes_read" << bytes_read << "\n";
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
CSVMatrix<T>::CSVMatrix(const char* file_path, bool forced_load) 
                    : file_stream_(file_path), file_name_(file_path), 
                    num_rows_(0), num_columns_(0), is_loaded_(false) {
  
  assert(file_stream_.is_open());
  if (!file_stream_.is_open())
    throw std::filesystem::filesystem_error("file failed to open", 
                                            std::error_code()); 

  file_byte_size_ = GetFileSize(file_path);

  if (!forced_load) 
    return;

  InitMatrix(); // inits rows and columns
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

  int rows = 0;
  while (!file_stream_.eof()) {
    file_stream_.getline(&buffer[0], buffer_size);
    LoadCSVLine(buffer, data_matrix_, file_stream_.gcount());
    buffer.replace(0, buffer_size, buffer_size, '\0');
    ++rows;
  }

  num_rows_ = rows;
  assert(data_matrix_.size() % num_rows_ == 0);
  num_columns_ = data_matrix_.size() / num_rows_;
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
  for (std::size_t i=0; i<num_rows_; i++) {
    for (std::size_t j=0; j<num_columns_; j++) {
      std::cout << data_matrix_[i*num_columns_ + j] << ", ";
    }
    std::cout << "\n";
  }
}