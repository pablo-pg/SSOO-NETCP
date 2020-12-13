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

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <sstream>

class File {
 public:
  File(std::string& file_name);
  ~File();

  std::vector<std::string> GetData() const;
  std::string GetStringData() const;

 private:
  void SetData_();
  std::string file_name_;
  std::ifstream file_input;
  std::vector<std::string> data_;
};

#endif  // FILE_H_
