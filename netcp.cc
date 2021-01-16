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
#include <cstring>
#include <iostream>
#include <string>
#include <system_error>
#include <thread>

#include "./socket.h"
#include "./message.h"
#include "./file.h"
#include "./main_functions.h"

std::atomic<bool> quit_tarea2(false), quit_tarea3(false), pause_send(false);

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
  try {
    std::string help_text = "--help", help_text2 = "-h";
    if (argc == 1) {
      std::thread tarea1_thread(tarea1);
      tarea1_thread.join();
    } else if (argc == 2) {
      if (argv[1] == help_text || argv[1] == help_text2) {
        help();
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
  std::string command;
  std::string receive_text = "receive";
  std::string send_text = "send";
  std::string abort_text = "abort";
  std::string pause_text = "pause";
  std::string resume_text = "resume";
  std::string quit_text = "quit";
  std::string help_text = "help";
  std::cout << "Bienvenido a mi Netcp, introduzca el comando.\nSi no sabe qué "
            << "comandos se pueden añadir, puede usar el comando \"help\" y se"
            << "los mostrará." << std::endl;
  std::thread tarea2, tarea3;
  std::exception_ptr eptr {};
  while (command != quit_text) {
    std::string second_word, first_world;
    std::cout << ">>  ";
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
    if (command == receive_text) {        //< RECIBIR
      std::cout << "Creando la carpeta " << second_word << "..." << std::endl;
      int fail = mkdir(second_word.c_str(), 0777);
      if (fail == -1) {
        std::cerr << "Netcp: No se pudo crear la carpeta: "
                  << std::strerror(errno) << "(" << errno << ")" << std::endl;
      } else {
        std::cout << "Carpeta creada." << std::endl;
      }
      tarea3 = std::thread(receive, std::ref(eptr), std::ref(quit_tarea2));
    } else if (command == send_text) {    //< ENVIAR
      tarea2 = std::thread(send_file, std::ref(eptr), second_word,
                           std::ref(quit_tarea2), std::ref(pause_send));
    } else if (command == abort_text) {
      if (second_word == receive_text) {  //< ABORT RECIBIR
        quit_tarea3 = true;
        // tarea3.join();
      } else {                            //< ABORT ENVIAR
        quit_tarea2 = true;
        std::cout << "Recepción abortada." << std::endl;
        // tarea2.join();
      }
    } else if (command == pause_text) {   //< PAUSAR ENVIAR
      pause_send = true;
      std::cout << "Envío pausado." << std::endl;
    } else if (command == resume_text) {  //< REANUDAR ENVIAR
      pause_send = false;
      std::cout << "Envío reanudado." << std::endl;
    } else if (command == help_text) {    //< HELP
      std::thread help_thread(help);
      help_thread.join();
    } else if (command == quit_text) {    //< QUIT
      quit_tarea2 = true;
      quit_tarea3 = true;
      tarea2.join();
      tarea3.join();
    }
    if (eptr) {
      std::cout << "EXCEPCION" << std ::endl;
      std::rethrow_exception(eptr);
    }
  }
  return 0;
}
