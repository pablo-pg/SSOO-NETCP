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
  file_fd_ = open(file_name.c_str(), O_RDONLY);
  if (file_fd_ == -1) {
    throw std::system_error(errno, std::system_category(),
                            "no se pudo abrir el fichero");
  }
  struct stat buf{};
  // Guardo en buf toda la info del archivo (size, permisos,...)
  fstat(file_fd_, &buf);
  file_size_ = buf.st_size;   // Guardo el tamaño del archivo
  file_info_ = buf;
  data_.resize(file_size_);
  Read();
}

File::File(const std::string& file_name, const struct stat& metadata) {
  file_name_ = file_name;
  int flags = O_CREAT | O_RDWR;
  file_fd_ = open(file_name.c_str(), flags, metadata.st_mode);
  if (file_fd_ == -1) {
    throw std::system_error(errno, std::system_category(),
                            "no se pudo abrir el fichero");
  }
  // Guardo en buf toda la info del archivo (size, permisos,...)
  file_size_ = metadata.st_size;   // Guardo el tamaño del archivo
  file_info_ = metadata;
  data_.resize(file_size_);
  int trunc = ftruncate(file_fd_, metadata.st_size);
  unmap_region_ = map<uint8_t>(PROT_WRITE, file_size_);
  if (trunc < 0) {
    throw std::system_error(errno, std::system_category(),
                            "no se puede establecer el tamaño del archivo");
  }
}

File::~File() {
close(file_fd_);
file_name_.clear();
data_.clear();
data_.resize(0);
data_.shrink_to_fit();
}


template<typename T> std::unique_ptr<T, std::function<void(T*)>>
File::map(int prot, size_t num, off_t offset) {
  // std::cout << "Valor de num: " << num * sizeof(T)<< std::endl << "Valor de offset" << offset << std::endl;
  mapped_mem_ = mmap(nullptr, num * sizeof(T), prot, MAP_SHARED, file_fd_,
                          offset);
  if (mapped_mem_ == MAP_FAILED) {
    throw std::system_error(errno, std::system_category(), "Fallo en mmap()");
  }
  auto mmap_deleter = [num](T* addr) {
  std::cout << "deleting map" << std::endl;
    munmap(
      addr,               // Puntero a la región a liberar (devuelto por mmap())
    num * sizeof(T));     // Tamaño de la porción a liberar. La Liberamos toda.
  };
  return { static_cast<T*>(mapped_mem_), mmap_deleter };
}


void File::Read() {
  // int sz;
  // sz = read(file_fd, &data_[0], file_size_);
  // std::cout << sz << std::endl;
  // if (sz < 0) {
  //   throw std::system_error(errno, std::system_category(),
  //                           "no se pudo leer el fichero");
  // }
  auto memory_region = map<uint8_t>(PROT_READ, file_size_);
  const uint8_t* memory_region_begin = memory_region.get();
  const uint8_t* memory_region_end = memory_region_begin + file_size_;
  uint position = 0;
  for (const uint8_t* p = memory_region_begin; p < memory_region_end; ++p) {
    data_.at(position) = *p;
    position++;
  }
}


void File::SetData(const std::string& text) {
  data_ = text;
  auto memory_region = map<uint8_t>(PROT_READ, file_size_);
  const uint8_t* memory_region_begin = memory_region.get();
  const uint8_t* memory_region_end = memory_region_begin + file_size_;
  uint position = 0;
  for (const uint8_t *p = memory_region_begin; p < memory_region_end; ++p) {
    // *p = data_.at(position);
    position++;
  }
}
