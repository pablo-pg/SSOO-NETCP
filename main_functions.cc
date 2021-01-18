/**
 * @file main_functions.cpp
 * @author Pablo Pérez González (alu0101318318@ull.edu.es)
 * @brief Funciones principales del main separadas para mejor organización
 * @version 0.1
 * @date 2021-01-06
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "./main_functions.h"


void help_function() {
  std::cout << "Práctica de Sistemas Operativos. Netcp\nEste programa envía y"
            << " recibe ficheros.\n\nAl ejecutar el programa se abrirá una "
            << "consola especial. Ahí introducirá los comandos que deseen "
            << "realizarse. Comandos:\n\n\n· receive [directorio]:​ activar el "
            << " modo de recepción. Con en el modo de recepción activado, el "
            << "programa puede recibir archivos de otro Netcp y guardarlos en ​"
            << "‘directorio’​, que debe ser creado si no existe. Cada vez que "
            << "un archivo termine de recibirse, se indicará que la descarga "
            << "ha finalizado y su ruta, mostrando un mensaje en la salida "
            << "estándar.\n· send [nombre_archivo]​: iniciará el envío del "
            << "archivo indicado. Cuando el archivo termine deenviarse, se "
            << "indicará con un mensaje en la salida estándar.\n· abort​: "
            << "abortará el envío. Si el comando es abort receive​, se "
            << "desactiva el modo de recepción.\n· pause:, pausará el envío.\n"
            << "· resume​: continuará el envío.\n· quit:​ termina la ejecución "
            << "del proceso de forma ordenada, cerrando los archivos, sockets y"
            << " el resto de recursos utilizados." << std::endl;
}


int send_file(std::exception_ptr& eptr, std::string argv,
              std::atomic<bool>& quit_tarea2, std::atomic<bool>& pause_send,
              std::atomic<bool>& quit_app) {
  try {
    if (quit_tarea2 || quit_app) {
      return 0;
    }
    while (pause_send) {
      std::this_thread::yield();
    }
    std::string filename;
    filename = argv;
    File file(filename);
    FileMetadata metadata;
    metadata = SetMetadata(file.GetData(), filename, file.GetMetaInfo());
    sockaddr_in address_to_send = make_ip_address(2000, "127.0.0.1");
    Socket remote(make_ip_address(3000, "127.0.0.3"));
    remote.send_to(metadata, address_to_send);
    for (int package {0}; package < metadata.packages_number; package++) {
      if (quit_tarea2 || quit_app) {
        std::cout << "Envío abortado." << std::endl;
        return 0;
      } else {
std::this_thread::sleep_for(std::chrono::seconds(2));
        while (pause_send) {
          std::this_thread::yield();
        }
        remote.send_to(file.GetMappedMem() + (package * MESSAGE_SIZE),
                      address_to_send, MESSAGE_SIZE);
        std::cout << "Paquete " << package+1 << "/" << metadata.packages_number
                  << " enviado." << std::endl;
      }
    }
    std::cout << "~ Archivo enviado. ~" << std::endl;
  }
  catch (const std::exception& e) {
    eptr = std::current_exception();
  }
  return 0;
}

int receive_file(std::exception_ptr& eptr, std::string folder,
                 std::atomic<bool>& quit_tarea3, std::atomic<bool>& quit_app) {
  try {
    if (!quit_app && !quit_tarea3) {
      Socket local(make_ip_address(2000, "127.0.0.1"));
      FileMetadata metadata;
      sockaddr_in address_to_receive = make_ip_address(3000, "127.0.0.3");
      if (!quit_app && !quit_tarea3) {
        metadata = local.receive_metadata(address_to_receive);
      }
      if (errno != EINTR) {
std::cout << "prueba1" << std::endl;
        std::string string_filename(metadata.filename.data());
        // El archivo donde se guardará el mensaje recibido
        File file("temp_exit.txt", metadata.file_info);
        for (int i {0}; i < metadata.packages_number - 1; i++) {
          if (quit_tarea3 || quit_app) {
            std::cout << "Recepción de mensaje abortada." << std::endl;
            return 0;
          } else {
            local.receive_from(address_to_receive,
                  file.GetMappedMem() + (i * MESSAGE_SIZE), MESSAGE_SIZE);
            std::cout << "Paquete " << i+1 << "/" << metadata.packages_number
                      << " recibido." << std::endl;
          }
        }
        if (metadata.file_size % MESSAGE_SIZE != 0) {
          local.receive_from(address_to_receive,
            file.GetMappedMem() + (metadata.packages_number - 1) * MESSAGE_SIZE,
            metadata.file_size % MESSAGE_SIZE);
          std::cout << "Paquete " << metadata.packages_number << "/"
                    << metadata.packages_number << " recibido." << std::endl;
        } else {
          local.receive_from(address_to_receive,
            file.GetMappedMem() + (metadata.packages_number - 1) * MESSAGE_SIZE,
            MESSAGE_SIZE);
          std::cout << "Paquete " << metadata.packages_number << "/"
                    << metadata.packages_number << " recibido." << std::endl;
        }
        std::cout << "~ Archivo recibido. ~" << std::endl;
        move_file(metadata.filename, folder);
      }
    }
  }
  catch (const std::exception& e) {
    eptr = std::current_exception();
  }
  return 0;
}


// FUNCIONES SECUNDARIAS


sockaddr_in make_ip_address(int port, const std::string& ip_address) {
  sockaddr_in direction{};
  direction.sin_family = AF_INET;
  direction.sin_port = htons(port);
  if (ip_address.empty()) {
    direction.sin_addr.s_addr = htonl(INADDR_ANY);
  } else {
    inet_aton(ip_address.c_str(), &direction.sin_addr);
  }
// std::cout << std::endl;
// std::cout << "Puerto: " << direction.sin_port << " (" << port << ")"
//           << std::endl;
// std::cout << "Direccion: " << direction.sin_addr.s_addr << " (" << ip_address
//           << ")" << std::endl << std::endl;
  if (port > 65525 || port < 1) {
    throw std::system_error(errno, std::system_category(),
                            "Puerto fuera de rango: " + port);
  }
  return direction;
}


FileMetadata SetMetadata(const std::string& text, const std::string& filename,
                          const struct stat& meta_info) {
  FileMetadata metadata;
  for (size_t i{0}; i < filename.size(); i++) {
    metadata.filename.at(i) = filename.at(i);
  }
  metadata.filename.at(filename.size()) = '\0';
  metadata.file_size = text.size();
  metadata.packages_number = metadata.calculate_message_num(metadata.file_size);
  metadata.file_info = meta_info;
  return metadata;
}


void move_file(const std::array<char, 1024UL>& file_name,
               const std::string & folder_name) {
  std::string cmd_str;
  cmd_str.append("mv ");
  cmd_str.append(get_current_dir_name());
  cmd_str.append("/");
  cmd_str.append("temp_exit.txt ");
  cmd_str.append(get_current_dir_name());
  cmd_str.append("/");
  cmd_str.append(folder_name);
  cmd_str.append("/");
  cmd_str.append(file_name.data());
  char* cmd;
  std::strcpy(cmd, cmd_str.c_str());
  system(cmd);
}



