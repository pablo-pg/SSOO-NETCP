/**
 * @file socket.h
 * @author Pablo Pérez González (alu0101318318@ull.edu.es)
 * @brief La clase Socket
 * @version 0.1
 * @date 2020-12-07
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef SOCKET_H_
#define SOCKET_H_

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <unistd.h>
#include <exception>
#include <iostream>
#include <array>
#include <vector>

#include "./message.h"

class Socket {
 public:
  explicit Socket(const sockaddr_in& address);
  ~Socket();

  void send_to(const Message& message, const sockaddr_in& address);
  // void send_to(const std::vector<uint8_t>& data, const sockaddr_in& address);
  void receive_from(Message& message, const sockaddr_in& address);
  // void receive_from(std::vector<uint8_t>& data, sockaddr_in& address);

  sockaddr_in get_address() const {return address_;}

 private:
  int fd_;
  sockaddr_in address_;
};



#endif  // SOCKET_H_
