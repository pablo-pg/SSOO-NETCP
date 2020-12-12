/**
 * @file message.h
 * @author Pablo Pérez González (alu0101318318@ull.edu.es)
 * @brief 
 * @version 0.1
 * @date 2020-12-12
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_


#include <iostream>
#include <array>
#include <vector>
#include <string>

#define PATH_MAX 1024

struct Message {
  size_t data_size;
  size_t file_size;
  std::array<char, PATH_MAX> filename;
  std::vector<char> data;    // El mensaje
};

// Message set_data(const std::string& text);

Message set_data(const std::string& text) {
  Message message;
  message.data_size = text.size();
  message.data.resize(message.data_size);
  for (size_t i {0}; i < text.size(); i++) {
    message.data.at(i) = text.at(i);
  }
  return message;
}

#endif  // MESSAGE_H_
