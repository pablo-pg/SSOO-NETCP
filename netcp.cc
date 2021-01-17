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

#include "./socket.h"
#include "./message.h"
#include "./file.h"
#include "./main_functions.h"


std::atomic<bool> quit_tarea2(false), quit_tarea3(false), pause_send(false),
                  quit_app(false);

int protected_main(const int& argc, char* argv[]);


void signal_handler(int signal) {
  std::string message;
  if (signal == SIGUSR1) {
    message = "Señal SIGUSR1 recibida.\n";
    quit_tarea2 = true;
  } else if (signal == SIGUSR2) {
    message = "Señal SIGUSR2 recibida.\n";
    quit_tarea3 = true;
  } else if (signal == SIGINT || signal == SIGHUP || signal == SIGTERM) {
    message = "Algo malo ha pasado. Cerrando el programa.\n";
    quit_app = true;
  }
  write(STDOUT_FILENO, message.c_str(), strlen(message.c_str()));
}

// MAIN
int main(int argc, char* argv[]) {
  struct sigaction signals = {0};
  signals.sa_handler = &signal_handler;
  sigaction(SIGUSR1, &signals, NULL);
  sigaction(SIGUSR2, &signals, NULL);
  sigaction(SIGINT, &signals, NULL);
  sigaction(SIGHUP, &signals, NULL);
  sigaction(SIGTERM, &signals, NULL);
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
  try {
    std::string help_text = "--help", help_text2 = "-h";
    if (argc == 1) {
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
  return 0;
}


int tarea1() {
  enum all_commands {send, receive, pause, abort, resume, quit, help};
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
            << "los mostrará." << std::endl;
  std::thread tarea2, tarea3;
  std::exception_ptr eptr {};
  while (command != "quit" || !quit_app) {
    std::string second_word, first_world;
    std::cout << ">>  ";
    std::getline(std::cin, command);
    size_t space_pos = command.find(' ');
    // ---> La lectura de comandos se puede mejorar
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
        tarea2 = std::thread(send_file, std::ref(eptr), second_word,
                           std::ref(quit_tarea2), std::ref(pause_send));
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
        tarea3 = std::thread(receive_file, std::ref(eptr),
                             std::ref(quit_tarea3));
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
            std::cout << "Recepción abortada." << std::endl;
            tarea3.join();
          } else {
            std::cout << "No se ha podido abortar." << std::endl;
          }
        }                                   //< ABORT ENVIAR
        if (tarea2.joinable()) {
          quit_tarea2 = true;
          tarea2.join();
        } else {
          std::cout << "No se ha podido abortar." << std::endl;
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
    // if (command == receive_text) {        //< RECIBIR
      // ----> Creo, hay funciones que crean y verifican si existe un directorio
      // std::cout << "Creando la carpeta " << second_word << "..." << std::endl;
      // int fail = mkdir(second_word.c_str(), 0777);
      // if (fail == -1) {
      //   std::cerr << "Netcp: No se pudo crear la carpeta: "
      //             << std::strerror(errno) << "(" << errno << ")" << std::endl;
      // } else {
      //   std::cout << "Carpeta creada." << std::endl;
      // }
      // std::string path = second_word;
      // tarea3 = std::thread(receive, std::ref(eptr), std::ref(quit_tarea3));
    // } else if (command == send_text) {    //< ENVIAR
    //   tarea2 = std::thread(send_file, std::ref(eptr), second_word,
    //                        std::ref(quit_tarea2), std::ref(pause_send));
    // } else if (command == abort_text) {
      // if (second_word == receive_text) {  //< ABORT RECIBIR
      //   if (tarea3.joinable()) {
      //     quit_tarea3 = true;
      //     std::cout << "Recepción abortada." << std::endl;
      //     tarea3.join();
      //   }
      // } else {                            //< ABORT ENVIAR
      //   if (tarea3.joinable()) {
      //     quit_tarea2 = true;
      //     std::cout << "Envío abortado." << std::endl;
      //     tarea2.join();
      //   }
      // }
    // } else if (command == pause_text) {   //< PAUSAR ENVIAR
      // pause_send = true;
      // std::cout << "Envío pausado." << std::endl;
    // } else if (command == resume_text) {  //< REANUDAR ENVIAR
      // pause_send = false;
      // std::cout << "Envío reanudado." << std::endl;
    // } else if (command == help_text) {    //< HELP
    //   std::thread help_thread(help);
    //   help_thread.join();
    // } else if (command == quit_text) {    //< QUIT
      // quit_tarea2 = true;
      // quit_tarea3 = true;
      // if (tarea2.joinable()) {
      //   tarea2.join();
      // }
      // if (tarea3.joinable()) {
      //   tarea3.join();
      // }
    if (eptr) {
      std::cout << "EXCEPCION" << std ::endl;
      std::rethrow_exception(eptr);
    }
  }
  return 0;
}
