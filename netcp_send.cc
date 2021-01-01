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

int main(int argc, char* argv[]) {
  try {
    std::string filename;
    if (argc == 2) {
      filename = argv[1];
    } else {
      throw std::invalid_argument("No se ha introducido fichero a enviar.");
    }
    File file(filename);
    FileMetadata metadata;
    Message message;
    metadata = SetMetadata(file.GetData(), filename);
    sockaddr_in address_to_send = make_ip_address(2000, "127.0.0.1");
    Socket remote(make_ip_address(3000, "127.0.0.3"));
    remote.send_to(metadata, address_to_send);
    for (int package {0}; package < metadata.packages_number; package++) {
      message = SetInfo(file.GetData(), package);
      remote.send_to(message, address_to_send);
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
  catch(std::invalid_argument& e) {
    std::cerr << "netcp: " << "introduzca solo 1 archivo que copiar" << "\n";
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
  // Primer paquete
  if (package == 0) {
    for (int i {0}; i < MESSAGE_SIZE - 1; i++) {
      if ((size_t)i < text.size()) {
        message.data.at(i) = text.at(i);
      } else {
        message.data.at(i) = '\0';
      }
    }
  } else {
  // Demás paquetes
    for (int i {0}; i < MESSAGE_SIZE - 1; i++) {
      if ((size_t)(i + MESSAGE_SIZE * package) < text.size()) {
        message.data.at(i) = text.at(i + (MESSAGE_SIZE * package) - 1);
      } else if ((size_t)(i + MESSAGE_SIZE * package) == text.size()) {
        message.data.at(i) = '\0';
      }
    }
  }
  message.data.at(MESSAGE_SIZE - 1) = '\0';
  return message;
}

