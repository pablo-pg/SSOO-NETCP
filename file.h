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

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <array>
#include <exception>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>


class File {
 public:
  explicit File(const std::string& file_name);          // Fichero que se envía
  File(const std::string& file_name, const struct stat& metadata);  // Se recibe
  ~File();

  std::string GetData() const {return data_;}
  struct stat GetMetaInfo() const {return file_info_;}
  void* GetMappedMem() const {return mapped_mem_;}
  int GetFileSize() const {return file_size_;}
//   void SetData(const std::string& text);

 private:
  void Read();  // Método que mapea en memoria
  template<typename T> std::unique_ptr<T, std::function<void(T*)>>
          map(int prot, size_t num = 1, off_t offset = 0);  // Mapeo de memoria
  int file_fd_;
  std::string file_name_;
  int file_size_;
  struct stat file_info_;
  std::string data_;
  void* mapped_mem_;
  std::unique_ptr<uint8_t, std::function<void(uint8_t*)>> unmap_region_;
};

#endif  // FILE_H_
