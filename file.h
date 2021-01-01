/**
 * @file file.h
 * @author Pablo Pérez González (alu0101318318@ull.edu.es)
 * @brief La clase File
 * @version 0.1
 * @date 2020-12-12
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef FILE_H_
#define FILE_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <exception>
#include <iostream>
#include <string>
#include <vector>
#include <array>


class File {
 public:
  explicit File(const std::string& file_name);
  ~File();

  std::string GetData() const;

 private:
  void Read();  // Método que lee
  std::string file_name_;
  int file_size_;
  int file_fd;
  std::string data_;
};

#endif  // FILE_H_
