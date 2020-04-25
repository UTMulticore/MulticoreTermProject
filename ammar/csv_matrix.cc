#include "csv_matrix.h"

#include <cassert>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>
#include <system_error>
#include <vector>

#include <sys/types.h>
#include <sys/stat.h>

// TODO: Check Style. Run a linter.

#define CONSOLE_LOG(x) std::cout << x << "\n";

// Relies on POSIX syscall ! I am favoring this over file seek to end, 
// since CSV files may be large.
static std::size_t GetFileSize(const char* path) {
  struct stat ret;
  int rc = stat(path, &ret);
  return rc == 0 ? ret.st_size : 0;  
}

// Given a CSV line, splits the values into a vector of elems. 
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


CSVMatrix::CSVMatrix(const char* file_path, bool forced_load) 
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
  num_rows_ = data_matrix_.size();
  num_columns_ = data_matrix_[0].size();
  is_loaded_ = true;
}


CSVMatrix::~CSVMatrix() {
  file_stream_.close();
}

// Loads the entire CSV file into the data_matrix_.
void CSVMatrix::InitMatrix() {
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

std::vector<std::string> CSVMatrix::sample() {
  if (!is_loaded_) 
    InitMatrix();

  assert(num_rows_ > 0);
  return data_matrix_[rand() % num_rows_];
}

std::vector<std::string> CSVMatrix::sample(std::size_t idx) {
  assert(idx < num_rows_);
  if (!is_loaded_) 
    InitMatrix();
  return data_matrix_[idx];
}


void CSVMatrix::dumpMatrix() {
  std::cout << "DIMS: " << num_rows_ << " " << num_columns_ << "\n";
  for (auto& row : data_matrix_) {
    for (auto& elem : row) {
      std::cout << elem << " ";
    }
    std::cout << "\n";
  } 
}