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

#include "./socket.h"

sockaddr_in make_ip_address(int port, const std::string& ip_address =
                            std::string());


int main() {
  try {
    Socket local(make_ip_address(2000, "127.0.0.1"));
    FileMetadata metadata;
    Message m_recibido;
    std::vector<Message> all_message;
    local.receive_from(metadata, make_ip_address(3000, "127.0.0.3"));
    std::cout << "Datos de:" << metadata.filename.data()
              << "\nSeparado en: " << metadata.packages_number
              << "\nTamaño: " << metadata.file_size << std::endl;
    sockaddr_in address = make_ip_address(3000, "127.0.0.3");
    for (int i {0}; i < metadata.packages_number; i++) {
      m_recibido = local.receive_from(address);
      std::cout << "\n\nPaquete:\n" << m_recibido.data.data() << std::endl;
      all_message.push_back(m_recibido);
    }
    std::cout << "Archivo completo:" << std::endl;
    for (Message message : all_message) {
      std::cout << message.data.data();
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
