/**
 * @file main.cc
 * @author Pablo Pérez González (alu0101318318@ull.edu.es)
 * @brief Main para hacer un netcp
 * @version 0.1
 * @date 2020-12-07
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <system_error>
#include <iostream>
#include <cstring>

#include "./socket.h"
#include "./message.h"
#include "./file.h"

sockaddr_in make_ip_address(int port, const std::string& ip_address =
                            std::string());


FileMetadata SetMetadata(const std::string& text, const std::string& filename);
Message SetInfo(const std::string& text, const int& package);

int main() {
  try {
    std::string filename = "in.txt";
    File file(filename);
    FileMetadata metadata;
    Message message;
    metadata = SetMetadata(file.GetStringData(), filename);
    Socket remote(make_ip_address(3000, "127.0.0.3"));
    remote.send_to(metadata, make_ip_address(2000, "127.0.0.1"));
    for (int package {0}; package < metadata.packages_number; package++) {
// std::cout << "Paquete: " << package << std::endl;
      message = SetInfo(file.GetStringData(), package);
      remote.send_to(message, make_ip_address(2000, "127.0.0.1"));
    }
  }
  catch(std::bad_alloc& e) {
    std::cerr << "netcp" << ": memoria insuficiente\n";
    return 1;
  }
  catch(std::system_error& e) {
    std::cerr << "netcp" << ": " << e.what() << '\n';
    return 2;
  }
  catch (...) {
    std::cout << "Error desconocido\n";
    return 99;
  }
  return 0;
}




sockaddr_in make_ip_address(int port, const std::string& ip_address) {
  sockaddr_in direction{};
  direction.sin_family = AF_INET;
  direction.sin_port = htons(port);
  if (ip_address.empty()) {
    direction.sin_addr.s_addr = htonl(INADDR_ANY);
  } else {
    inet_aton(ip_address.c_str(), &direction.sin_addr);
  }
std::cout << std::endl;
std::cout << "Puerto: " << direction.sin_port << " (" << port << ")"
          << std::endl;
std::cout << "Direccion: " << direction.sin_addr.s_addr << " (" << ip_address
          << ")" << std::endl << std::endl;
  if (port > 65525 || port < 1) {
    throw std::system_error(errno, std::system_category(),
                            "Puerto fuera de rango: " + port);
  }
  return direction;
}


FileMetadata SetMetadata(const std::string& text, const std::string& filename) {
  FileMetadata metadata;
  for (size_t i{0}; i < filename.size(); i++) {
    metadata.filename.at(i) = filename.at(i);
  }
  metadata.filename.at(filename.size()) = '\0';
  metadata.file_size = text.size();
  metadata.packages_number = metadata.calculate_message_num(metadata.file_size);
  return metadata;
}

Message SetInfo(const std::string& text, const int& package) {
  Message message;
  int pack = package + 1;
  // Primer paquete
  if (package == 0) {
    for (int i {0}; i < MESSAGE_SIZE; i++) {
      if ((size_t)i < text.size()) {
        message.data.at(i) = text.at(i);
      }
    }
  } else {
  // Demás paquetes
    for (int i {0}; i < MESSAGE_SIZE; i++) {
      if ((size_t)(i + MESSAGE_SIZE * package) < text.size()) {
        message.data.at(i) = text.at(i + MESSAGE_SIZE * package);
      } else if ((size_t)(i + MESSAGE_SIZE * package) == text.size()) {
        message.data.at(i) = '\0';
      }
    }
  }
  return message;
}

