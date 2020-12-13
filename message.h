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
  std::array<char, 1024> data;    // El mensaje
};

#endif  // MESSAGE_H_
