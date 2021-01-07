/**
 * @file main_functions.h
 * @author Pablo Pérez González (alu0101318318@ull.edu.es)
 * @brief Funciones principales del main separadas para mejor organización
 * @version 0.1
 * @date 2021-01-06
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef MAIN_FUNCTIONS_H_
#define MAIN_FUNCTIONS_H_

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <system_error>
#include <iostream>
#include <cstring>
#include <string>

#include "./socket.h"
#include "./message.h"
#include "./file.h"


void help();
int send(const char* argv);
int receive();

sockaddr_in make_ip_address(int port, const std::string& ip_address =
                            std::string());

FileMetadata SetMetadata(const std::string& text, const std::string& filename,
                          const struct stat& meta_info);
Message SetInfo(const std::string& text, const int& package);


#endif  // MAIN_FUNCTIONS_H_
