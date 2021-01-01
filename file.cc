/**
 * @file file.cc
 * @author Pablo Pérez González (alu0101318318@ull.edu.es)
 * @brief La clase File
 * @version 0.1
 * @date 2020-12-12
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "./file.h"

File::File(const std::string& file_name) {
  file_name_ = file_name;
  file_fd = open(file_name.c_str(), O_RDONLY);
  if (file_fd == -1) {
    throw std::system_error(errno, std::system_category(),
                            "no se pudo abrir el fichero");
  }
  struct stat buf{};
  fstat(file_fd, &buf);
  file_size_ = buf.st_size;
  data_.resize(file_size_);
  Read();
}

File::~File() {
close(file_fd);
file_name_.clear();
data_.clear();
data_.resize(0);
data_.shrink_to_fit();
}

void File::Read() {
  int sz;
  sz = read(file_fd, &data_[0], file_size_);
  std::cout << sz << std::endl;
  if (sz < 0) {
    throw std::system_error(errno, std::system_category(),
                            "no se pudo leer el fichero");
  }
}

std::string File::GetData() const {return data_;}
