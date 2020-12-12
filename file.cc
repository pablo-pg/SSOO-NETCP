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

File::File(std::string& file_name) {
  file_name_ = file_name;
  file_input.open(file_name);
  data_.resize(0);
  SetData_();
}

File::~File() {
if (file_input.is_open())
  file_input.close();
data_.clear();
data_.resize(0);
data_.shrink_to_fit();
}

void File::SetData_() {
  std::string line;
  while (getline(file_input, line)) {
    data_.push_back(line);
  }
}

std::vector<std::string> File::GetData() const {return data_;}

std::string File::GetStringData() const {
  std::stringstream all_lines;
  for (std::string line : data_) {
    all_lines << line;
    all_lines << "\n";
  }
  return all_lines.str();
}

// std::string Message::GetStringMessage() const {
//   std::string message(data_.begin(), data_.end());
//   return message;
// }

// void Message::SetSize(const int size) {
//   data_size_ = static_cast<size_t>(size);
//   data_.resize(data_size_);
// }

// void Message::SetMessage(const std::string& text) {
//   std::copy(begin(text), end(text), begin(data_));
//   data_.at(text.size()) = '\0';
//   for (size_t i = text.size(); i < data_.size(); i++) {
//     data_.at(i) = 0;
//   }
// }
