/**
 * @file socket.cc
 * @author Pablo Pérez González (alu0101318318@ull.edu.es)
 * @brief La clase Socket
 * @version 0.1
 * @date 2020-12-07
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "./socket.h"

Socket::Socket(const sockaddr_in& address) {
  // Se crea el socket
  fd_ = socket(AF_INET, SOCK_DGRAM, 0);
  address_ = address;
  if (fd_ < 0) {
    throw std::system_error(errno, std::system_category(),
                            "no se pudo crear el socket");
  } else {
    // std::cout << "(flag) socket num: " << fd_ << std::endl;
  }
  // Se asigna la dirección al socket
  int result = bind(fd_, reinterpret_cast<const sockaddr*>(&address),
                    sizeof(address));
  if (result < 0) {
    throw std::system_error(errno, std::system_category(),
                            "falló bind");
  } else {
    // std::cout << "(flag) bind error: " << result << std::endl;
  }
}

Socket::~Socket() {
  close(fd_);
// std::cout << "close (fd): " << fd_ << std::endl;
}

void Socket::send_to(const void* message, const sockaddr_in& address,
                     const int& size) {
  int result = sendto(fd_, message, size, 0,
                      reinterpret_cast<const sockaddr*>(&address),
                      sizeof(address));
  if (result < 0) {
    throw std::system_error(errno, std::system_category(),
                            "no se pudo enviar el mensaje");
  }
}

void Socket::send_to(const FileMetadata& metadata, const sockaddr_in& address) {
  int result = sendto(fd_, &metadata, sizeof(metadata), 0,
                      reinterpret_cast<const sockaddr*>(&address),
                      sizeof(address));
  if (result < 0) {
    throw std::system_error(errno, std::system_category(),
                            "no se pudo enviar los datos del archivo");
  } else {
    // std::cout << "Datos enviado del archivo:\"" << metadata.filename.data()
    //           << "\"" << std::endl;
  }
}

void Socket::receive_from(const sockaddr_in& address, void* mem_zone,
                          const int& size) {
  // std::cout << "Esperando mensaje..." << std::endl;
  socklen_t src_len = sizeof(address);
  sockaddr_in remote_address = address;
  int result = recvfrom(fd_, mem_zone, size, 0,
                      reinterpret_cast<sockaddr*>(&remote_address), &src_len);
  if (result < 0) {
    throw std::system_error(errno, std::system_category(),
                            "no se pudo recibir el mensaje");
  }
}

FileMetadata Socket::receive_metadata(const sockaddr_in& address) {
  FileMetadata metadata;
  std::cout << "Esperando datos..." << std::endl;
  socklen_t src_len = sizeof(address);
  sockaddr_in remote_address = address;
  int result = recvfrom(fd_, &metadata, sizeof(metadata), 0,
                      reinterpret_cast<sockaddr*>(&remote_address), &src_len);
  if (result < 0) {
    throw std::system_error(errno, std::system_category(),
                            "no se pudieron recibir los datos");
  } else {
    // std::cout << "Datos recibidos" << std::endl;
    return metadata;
  }
}

