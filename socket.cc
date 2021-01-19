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

void Socket::send_to(const void* mem_zone, const sockaddr_in& address,
                     const int& size) {
  Message message;
  sockaddr_in socket_data;
  socklen_t addrlen = sizeof(socket_data);
getsockname(fd_, reinterpret_cast<sockaddr*>(&socket_data), &addrlen);
  message.port = socket_data.sin_port;
  message.direction = socket_data.sin_addr.s_addr;
std::cout << "Puerto: " << socket_data.sin_port << std::endl;
  memcpy(message.msg, mem_zone, size - overhead);
  int result = sendto(fd_, &message, size, 0,
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
  socklen_t* src_len = NULL;
  // sockaddr_in remote_address = NULL;
  sockaddr* remote_address = NULL;
  //   socklen_t src_len = sizeof(address);
  // sockaddr_in remote_address = address;
  Message message;
  int result = recvfrom(fd_, &message, size, 0,
                    remote_address,  src_len);
  memcpy(mem_zone, message.msg, size - overhead);
  std::cout << "Puerto: " << message.port << std::endl;

    // if (remote_address == NULL)
  // std::cout << "remote: " << remote_address << " - Src_len: " << src_len << std::endl;

//   int result = recvfrom(fd_, mem_zone, size, 0,
                      // reinterpret_cast<sockaddr*>(&remote_address), &src_len);
  if (result < 0) {
    // if (errno != EINTR) {
      throw std::system_error(errno, std::system_category(),
                            "no se pudo recibir el mensaje");
    // }
  }
}

FileMetadata Socket::receive_metadata(const sockaddr_in& address) {
  FileMetadata metadata;
  std::cout << "Esperando datos..." << std::endl;
  socklen_t src_len = sizeof(address);
  sockaddr_in remote_address = address;
  int result = recvfrom(fd_, &metadata, sizeof(metadata), 0,
                      NULL, NULL);
  // int result = recvfrom(fd_, &metadata, sizeof(metadata), 0,
  //                     reinterpret_cast<sockaddr*>(&remote_address), &src_len);
  if (result < 0) {
    // if (errno != EINTR) {
      throw std::system_error(errno, std::system_category(),
                            "no se pudieron recibir los datos");
    // }
  } else {
    return metadata;
  }
}

