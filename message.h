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
  size_t file_size;
  int calculate_message_num(const size_t& size) {
    int num = 0;
    num = size / 1024;
    if ((size % 1024) != 0) {
      num++;
    }
    return num;
  }
  int packages_number = calculate_message_num(file_size);
};

// int calculate_nessage_num(const size_t& size) {
//   int num = 0;
//   num = size / 1024;
//   if ((size % 1024) != 0) {
//     num++;
//   }
//   return num;
// }


struct Message {
  const size_t size = MESSAGE_SIZE;
  std::array<char, MESSAGE_SIZE> data;
  Message& operator=(const Message rhs) {
    data = rhs.data;
    return *this;
  }
};

// class Message {
//  public:
//   Message(const std::string& data);
//   virtual void* data() = 0;
//   size_t size = 0;
//  private:

// };

// class FileMetadataMessage : public Message {
//  public:
//   FileMetadataMessage(const std::string& data) : Message(data){}
//   virtual void* data() { return &data_; }
//   virtual size_t size() { return sizeof(data_); }
//  private:
//   FileMetadataMessage *data_;
// };

// class DataMessage : public Message {
//  public:
//   DataMessage();
//   void* data() override { return data_.data(); }
//   size_t size() override { return data_.size(); }
//  private:
//   std::vector<uint8_t> data_;
// };

#endif  // MESSAGE_H_
