#include "data.h"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include <sys/types.h>
#include <sys/stat.h>

// TODO: Style fix. Run a linter.

#define CONSOLE_LOG(x) std::cout << x << "\n";

// Relies on POSIX syscall ! 
static std::size_t GetFileSize(const char* path) {
  struct stat ret;
  int rc = stat(path, &ret);
  return rc == 0 ? ret.st_size : 0;  
}

// Given a CSV line, splits the values into a vector of elems. 
static void LoadCSVLine(std::string& csv_line, std::vector<std::string>& matrix_row) {
  std::size_t position = 0;
  std::size_t end_position = 0;
  while(end_position != std::string::npos) {
    end_position = csv_line.find(',', position);
    matrix_row.push_back(csv_line.substr(position, end_position-position));
    position = end_position + 1;
  }
}

CSVMatrix::CSVMatrix(const char* file_path, bool forced_load) 
                    : file_name_(file_path), file_stream_(file_path) { // to init cols here or not ?
  
  assert(file_stream_.is_open());
  file_byte_size_ = GetFileSize(file_path);
  std::cout << file_byte_size_ << "\n";

  if (!forced_load) {
    return;
  }

  // load matrix
  std::size_t buffer_size =300;
  std::string buffer;
  buffer = "hi\n";
  buffer.resize(buffer_size);

  file_stream_.getline(&buffer[0], buffer_size);
  CONSOLE_LOG(buffer);

  // For now, we make the assumption that the first line is column names,
  // and that every column is named. Thus we can figure out the column size.
  
  // data_matrix_ aint even inited yet tho soooo?
  std::vector<std::string> row;
  LoadCSVLine(buffer, row);
  for (auto& i : row)
    CONSOLE_LOG(i);

}

std::vector<std::string> CSVMatrix::sample() {
  return data_matrix_[rand() % getRows()];
}

std::vector<std::string> CSVMatrix::sample(std::size_t idx) {
  return data_matrix_[idx];

}
