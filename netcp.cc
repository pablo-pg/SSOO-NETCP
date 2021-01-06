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
#include "./main_functions.h"


// MAIN
int main(int argc, char* argv[]) {
  std::string help_text = "--help";
  std::string help_text2 = "-h";
  std::string receive_text = "receive";
  std::string send_text = "send";
  try {
    if (argc == 1) {
      throw std::invalid_argument("No se ha indicado ninguna operación.\n"
                                  "Use el parámetro \"-h\" para leer ayuda.");
    }
    if (argc == 2) {
      if (argv[1] == help_text || argv[1] == help_text2) {
        help();
      } else if (argv[1] == receive_text) {
        receive();
      } else if (argv[1] == send_text) {
        throw std::invalid_argument("No ha indicado ningún archivo que enviar."
                                    "\n");
      } else {
        throw std::invalid_argument("La operación indicada no es válida.\n"
                                  "Use el parámetro \"-h\" para leer ayuda.");
      }
    } else if (argc == 3) {
      if (argv[1] == send_text) {
        send(argv[2]);
      } else {
        throw std::invalid_argument("La operación indicada no es válida.\n"
                                  "Use el parámetro \"-h\" para leer ayuda.");
      }
    } else {
      throw std::invalid_argument("Se han introducido demasiados argumentos.\n"
                            "Este programa solo permite enviar un fichero.\n"
                            "Use el parámetro \"-h\" para leer ayuda.");
    }
  }
  catch(std::invalid_argument& e) {
    std::cerr << "netcp: " << e.what() << std::endl;
  }
}
