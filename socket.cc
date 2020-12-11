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
  if (fd_ < 0) {
    throw std::system_error(errno, std::system_category(),
                            "no se pudo crear el socket");
  } else {
    std::cout << "(flag) socket num: " << fd_ << std::endl;
  }
  // Se asigna la dirección al socket
  int result = bind(fd_, reinterpret_cast<const sockaddr*>(&address),
                    sizeof(address));
  if (result < 0) {
    throw std::system_error(errno, std::system_category(),
                            "falló bind");
  } else {
    std::cout << "(flag) bind num: " << result << std::endl;
  }
}

Socket::~Socket() {
  close(fd_);
std::cout << "close (fd): " << fd_ << std::endl;
}

void Socket::send_to(const Message& message, const sockaddr_in& address) {
  int result = sendto(fd_, &message, sizeof(message), 0,
                      reinterpret_cast<const sockaddr*>(&address),
                      sizeof(address));
  if (result < 0) {
    throw std::system_error(errno, std::system_category(),
                            "no se pudo enviar el mensaje");
  } else {
    std::cout << "Mensaje enviado" << std::endl;
  }
}

void Socket::receive_from(Message& message, sockaddr_in& address) {
  socklen_t src_len = sizeof(address);
  int result = recvfrom(fd_, &message, sizeof(message), 0,
                      reinterpret_cast<sockaddr*>(&address), &src_len);
  if (result < 0) {
    throw std::system_error(errno, std::system_category(),
                            "no se pudo recibir el mensaje");
  }
}

