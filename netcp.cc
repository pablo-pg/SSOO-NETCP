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

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <atomic>
#include <csignal>
#include <cstring>
#include <iostream>
#include <map>
#include <string>
#include <system_error>
#include <thread>
#include <utility>
#include <unordered_map>

#include "./socket.h"
#include "./message.h"
#include "./file.h"
#include "./main_functions.h"


std::atomic<bool> quit_tarea2{false}, quit_tarea3{false}, pause_send{false },
                  quit_app{false};

int protected_main(const int& argc, char* argv[]);

// MAIN
int main(int argc, char* argv[]) {
  try {
    protected_main(argc, argv);
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
    std::cerr << "netcp: " << e.what() << std::endl;
    return 3;
  }
}

int tarea1();

int protected_main(const int& argc, char* argv[]) {
  if (!quit_app) {
    try {
      std::string help_text = "--help", help_text2 = "-h";
      if (argc == 1) {
        /// SE ESTABLECEN LAS VARIABLES DE ENTORNO
        setenv("NETCP_DEST_IP", "127.0.0.1", 0);
        setenv("NETCP_DEST_PORT", "2000", 0);
        setenv("NETCP_IP", "127.0.0.3", 0);
        setenv("NETCP_PORT", "3000", 0);
        /// En el caso de señales críticas
        sigset_t set;
        sigemptyset(&set);
        sigaddset(&set, SIGINT);
        sigaddset(&set, SIGTERM);
        sigaddset(&set, SIGHUP);
        sigaddset(&set, SIGUSR1);
        sigprocmask(SIG_BLOCK, &set, NULL);
        /// Se llama a la función encargada de controlar las señales críticas.
        std::thread sign_thread(signal_handler, std::ref(set));
        sign_thread.detach();
        /// Para el caso de la señal SIGUSR1
        struct sigaction act = {0};
        sigaction(SIGUSR1, &act, NULL);
        act.sa_handler = &usr1_funct;
        /// PROGRAMA PRINCIPAL - Hilo tarea1
        std::thread tarea1_thread(tarea1);
        tarea1_thread.join();
      } else if (argc == 2) {
        if (argv[1] == help_text || argv[1] == help_text2) {
          help_function();
        }
      } else {
        throw std::invalid_argument("Se han introducido demasiados argumentos.\n"
                              "Este programa no requiere argumentos.\n"
                              "Use el parámetro \"-h\" para leer ayuda.");
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
      std::cerr << "netcp: " << e.what() << std::endl;
      return 3;
    }
  }
  return 0;
}


int tarea1() {
  /// Hago un enum con todoas loas posibles comandos.
  enum all_commands {send = 1, receive, pause, abort, resume, quit, help};
  std::map<std::string, all_commands> registred_commands;
  registred_commands["send"] = send;
  registred_commands["receive"] = receive;
  registred_commands["pause"] = pause;
  registred_commands["abort"] = abort;
  registred_commands["resume"] = resume;
  registred_commands["quit"] = quit;
  registred_commands["help"] = help;
  std::string command;
  std::cout << "Bienvenido a mi Netcp, introduzca el comando.\nSi no sabe qué "
            << "comandos se pueden añadir, puede usar el comando \"help\" y se"
            << " los mostrará." << std::endl;
  std::thread tarea2, tarea3;             //< Los hilos de envío y recepción
  std::vector<std::pair<int, std::thread>> table;  //< Vector de hilos multiarch
  int count = 0;
  std::exception_ptr eptr {};
  while (command != "quit" || !quit_app) {
    std::string second_word, first_world;
    std::cout << ">>  ";
    if (quit_app) {
      return 0;
    }
    std::getline(std::cin, command);
    size_t space_pos = command.find(' ');
    if (space_pos != std::string::npos) {
      for (size_t pos = space_pos; pos < command.size(); ++pos) {
        if (command[pos] != ' ') {
          second_word += command[pos];
        }
      }
      for (size_t pos {0}; pos < space_pos; ++pos) {
        if (command[pos] != ' ') {
          first_world += command[pos];
        }
      }
      command = first_world;
    }
    /// LOS COMANDOS
    switch (registred_commands[command]) {
      case send:                            //< ENVIAR
      {
        count++;
table.push_back(std::make_pair(count, std::thread(send_file, std::ref(eptr), second_word,
                           std::ref(quit_tarea2), std::ref(pause_send),
                           std::ref(quit_app))));
        std::cout << "1id? " << table[0].second.get_id() << std::endl;
        std::cout << "Lista de hilos: " << std::endl;
        for (size_t i = 0; i < table.size(); i++) {
          std::cout << "Id: " << table[i].first << std::endl;
          if (table[i].second.joinable()) {
            std::cout << "El hilo " << table[i].first << " puede terminar" << std::endl;
            table[i].second.join();
            // table[i].first = 0;
          }
        }
        std::cout << "2id? " << table[0].second.get_id() << std::endl;
        break;
      }
      case receive:                         //< RECIBIR
      {
        std::cout << "Creando la carpeta " << second_word << "..." << std::endl;
        int fail = mkdir(second_word.c_str(), 0777);
        if (fail == -1) {
          std::cerr << "Netcp: No se pudo crear la carpeta: "
                    << std::strerror(errno) << "(" << errno << ")" << std::endl;
        } else {
          std::cout << "Carpeta creada." << std::endl;
        }
        tarea3 = std::thread(receive_file, std::ref(eptr), second_word,
                             std::ref(quit_tarea3), std::ref(quit_app));
        // kill(tarea3.native_handle(), SIGUSR1);
        break;
      }
      case pause:                           //< PAUSE
      {
        pause_send = true;
        std::cout << "Envío pausado." << std::endl;
        break;
      }
      case abort:
      {
        if (second_word == "receive") {     //< ABORT RECIBIR
          if (tarea3.joinable()) {
            quit_tarea3 = true;
            pthread_kill(tarea3.native_handle(), SIGUSR1);
            // std::cout << "Recepción abortada." << std::endl;
            tarea3.join();
          } else {
            std::cout << "No se ha podido abortar." << std::endl;
          }
        }                                   //< ABORT ENVIAR
        if (tarea2.joinable()) {
          quit_tarea2 = true;
          tarea2.join();
        } else {
          std::cout << "No se ha podido abortar, hilo en desuso." << std::endl;
        }
        break;
      }
      case resume:                          //< RESUME
      {
        pause_send = false;
        std::cout << "Envío reanudado." << std::endl;
        break;
      }
      case quit:                            //< QUIT
      {
        quit_tarea2 = true;
        quit_tarea3 = true;
        if (tarea2.joinable()) {
          tarea2.join();
        }
        if (tarea3.joinable()) {
          tarea3.join();
        }
        return 0;
std::cout << "Saliendo..." << std::endl;
        break;
      }
      case help:                            //< HELP
      {
        std::thread help_thread(help_function);
        help_thread.join();
        break;
      }
      default:
      {
        std::cout << "Comando no válido, vuelva a intentarlo." << std::endl;
      }
    }
    if (eptr) {
      std::cout << "EXCEPCION" << std ::endl;
      std::rethrow_exception(eptr);
    }
  }
  return 0;
}
