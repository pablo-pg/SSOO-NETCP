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
#include <csignal>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <unordered_map>

#include "./file.h"
#include "./message.h"
#include "./socket.h"


// std::atomic<bool> quit_tarea2{false}, quit_tarea3{false}, pause_send{false },
//                   quit_app{false};


void help_function();
int send_file(std::exception_ptr& eptr, std::string argv,
              std::atomic<bool>& quit_tarea2, std::atomic<bool>& pause_send,
              std::atomic<bool>& quit_app, const uint32_t& direction,
              const in_port_t port);
int receive_file(std::exception_ptr& eptr, std::string folder,
              std::atomic<bool>& quit_tarea3, std::atomic<bool>& quit_app);


/// CONTROLADOR DE SEÑALES
void signal_handler(const sigset_t& set);
void usr1_funct(int signal);

/// FUNCIONES SECUNDARIAS.

sockaddr_in make_ip_address(int port, const std::string& ip_address =
                            std::string());

FileMetadata SetMetadata(const std::string& text, const std::string& filename,
                         const struct stat& meta_info, const uint32_t& dir,
                         const in_port_t& the_port);

void move_file(const std::array<char, 1024UL>& file_name,
               const std::string& folder_name);

void make_send(std::exception_ptr& eptr, std::string argv,
              std::atomic<bool>& quit_tarea2, std::atomic<bool>& pause_send,
              std::atomic<bool>& quit_app, const uint32_t& direction,
              const in_port_t port);

void make_receive(std::exception_ptr& eptr, std::string folder,
              std::atomic<bool>& quit_tarea3, std::atomic<bool>& quit_app);

/// STRUCTS

// template <typename F, typename U/*, typename V*/>
struct SendingTask{
//  public:
  // inlindeSendingTask(const std::thread&) {

  // }
//  private:
  std::unique_ptr<std::thread> send_thread;
  // std::unique_ptr<bool> sent(new bool(false));
  // V pause_send_task{false};
  // V abort_send_task{false};
};

struct ReceptionTask {
  int fd;
  uint size;
  void* mem_zone;
};


struct pair_hash {
  template <class T1, class T2>
  std::size_t operator () (const std::pair<T1,T2> &p) const {
    auto h1 = std::hash<T1>{}(p.first);
    auto h2 = std::hash<T2>{}(p.second);
    return h1 ^ h2;  
  }
};
#endif  // MAIN_FUNCTIONS_H_
