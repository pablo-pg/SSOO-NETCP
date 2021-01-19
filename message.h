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
  uint32_t direction;
  in_port_t port;
  int calculate_message_num(const size_t& size) {
    int num = 0;
    num = size / MESSAGE_SIZE;
    if ((size % MESSAGE_SIZE) != 0) {
      num++;
    }
    int overhead = sizeof(uint32_t) + sizeof(in_port_t);
    int total_size = overhead * num + size;
    int total_packages = total_size / MESSAGE_SIZE;
    if ((total_size % MESSAGE_SIZE) != 0) {
      total_packages++;
    }
    return total_packages;
  }
  int packages_number = calculate_message_num(file_size);
};

struct Message {
  uint32_t direction;
  in_port_t port;
  void * zone_mem;
}

#endif  // MESSAGE_H_
