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
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <system_error>
#include <atomic>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>

#include "./file.h"
#include "./message.h"
#include "./socket.h"


void help_function();
int send_file(std::exception_ptr& eptr, std::string argv,
              std::atomic<bool>& quit_tarea2, std::atomic<bool>& pause_send,
              std::atomic<bool>& quit_app);
int receive_file(std::exception_ptr& eptr, std::string folder,
              std::atomic<bool>& quit_tarea3, std::atomic<bool>& quit_app);

/// FUNCIONES SECUNDARIAS.

sockaddr_in make_ip_address(int port, const std::string& ip_address =
                            std::string());

FileMetadata SetMetadata(const std::string& text, const std::string& filename,
                          const struct stat& meta_info);

void move_file(const std::array<char, 1024UL>& file_name,
               const std::string& folder_name);

void make_send(std::exception_ptr& eptr, std::string argv,
              std::atomic<bool>& quit_tarea2, std::atomic<bool>& pause_send,
              std::atomic<bool>& quit_app);


/// STRUCTS

struct SendingTask{
  std::thread send_thread;
  bool sent = 0;
};

#endif  // MAIN_FUNCTIONS_H_
