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
#include <netinet/in.h>
#include <cerrno>       // para errno
#include <cstring>      // para std::strerror()
#include <exception>
#include <iostream>
#include <array>

#define kSizeofMessage 1024

struct Message {
  std::array<char, kSizeofMessage> text;    // El mensaje
};



class Socket {
 public:
  explicit Socket(const sockaddr_in& address);
  ~Socket();

  void send_to(const Message& message, const sockaddr_in& address);
  void receive_from(Message& message, sockaddr_in& address);

 private:
  int fd_;
};



#endif  // SOCKET_H_
