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
#include <netinet/in.h>
#include <iostream>

#include "./socket.h"

sockaddr_in make_ip_address(int port, const std::string& ip_address =
                            std::string());


int main() {
  std::cout << "Hello World!" << std::endl;
}
