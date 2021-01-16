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

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <system_error>
#include <atomic>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>

#include "./file.h"
#include "./message.h"
#include "./socket.h"


void help();
int send_file(std::exception_ptr& eptr, std::string argv,
              std::atomic<bool>& quit_tarea2, std::atomic<bool>& pause_send);
int receive(std::exception_ptr& eptr, std::atomic<bool>& quit_tarea3);

sockaddr_in make_ip_address(int port, const std::string& ip_address =
                            std::string());

FileMetadata SetMetadata(const std::string& text, const std::string& filename,
                          const struct stat& meta_info);


#endif  // MAIN_FUNCTIONS_H_
