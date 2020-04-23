#include "csv_matrix.h"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include <sys/types.h>
#include <sys/stat.h>

// TODO: Style fix. Run a linter.

#define CONSOLE_LOG(x) std::cout << x << "\n";

// Relies on POSIX syscall ! I am favoring this over manual count, since CSV files may be large.=
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
  std::size_t buffer_size = 500; // <---- This had to be tweaked. How do I choose this in the future when I don't know how long a line may be ?
  std::string buffer;
  buffer.resize(buffer_size);

  file_stream_.getline(&buffer[0], buffer_size);
  CONSOLE_LOG(buffer);

  std::vector<std::string> row;
  LoadCSVLine(buffer, row);
  for (auto& i : row)
    CONSOLE_LOG(i);
  data_matrix_.push_back(std::move(row));

  buffer.replace(0, buffer_size, buffer_size, '\0');
  std::cout << buffer << "\n";
  file_stream_.getline(&buffer[0], buffer_size);
  std::cout << buffer << "\n";

  std::vector<std::string> row2;
  LoadCSVLine(buffer, row2);

  CONSOLE_LOG(row2.size());
  for (auto& str : row2) {
    CONSOLE_LOG(str);
  }
  // works for the first two but fails somewhere inbetween


  std::cout << "here!\n";
  while (!file_stream_.eof()) {
    std::vector<std::string> row;
    buffer.replace(0, buffer_size, buffer_size, '\0');
    std::cout << buffer << std::endl;
    file_stream_.getline(&buffer[0], buffer_size);
    std::cout << buffer << std::endl;
    // LoadCSVLine(buffer, row);
    // data_matrix_.push_back(std::move(row));
  }

  // For now, we make the assumption that the first line is column names,
  // and that every column is named. Thus we can figure out the column size.
  
  // // Print the whole matrix
  // for (auto& row : data_matrix_) { 
  //   for (auto& elem : row)
  //     std::cout << elem << " ";
  //   std::cout << std::endl;
  // }


}

std::vector<std::string> CSVMatrix::sample() {
  return data_matrix_[rand() % getRows()];
}

std::vector<std::string> CSVMatrix::sample(std::size_t idx) {
  return data_matrix_[idx];

}
