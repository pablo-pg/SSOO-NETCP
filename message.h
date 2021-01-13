/**
 * @file message.h
 * @author Pablo Pérez González (alu0101318318@ull.edu.es)
 * @brief Las clases referentes al mensaje
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
#define MESSAGE_SIZE 1024

struct FileMetadata {
  std::array<char, PATH_MAX> filename;
  struct stat file_info;
  size_t file_size;
  int calculate_message_num(const size_t& size) {
    int num = 0;
    num = size / MESSAGE_SIZE;
    if ((size % MESSAGE_SIZE) != 0) {
      num++;
    }
    return num;
  }
  int packages_number = calculate_message_num(file_size);
};



// struct Message {
//   std::array<char, MESSAGE_SIZE> data;
//   Message& operator=(const Message rhs) {
//     data = rhs.data;
//     return *this;
//   }
// };

#endif  // MESSAGE_H_
