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


void help() {
  std::cout << "Práctica de Sistemas Operativos. Netcp\nEste programa envía y"
            << " recibe ficheros.\n\nPara enviar use:\t./netcp send FICHERO\n"
            << "Para recibir use:\t./netcp receive" << std::endl;
  exit(0);
}


int send(const char* argv) {
  try {
    std::string filename;
    filename = argv;
    File file(filename);
    FileMetadata metadata;
    Message message;
    metadata = SetMetadata(file.GetData(), filename, file.GetMetaInfo());
    sockaddr_in address_to_send = make_ip_address(2000, "127.0.0.1");
    Socket remote(make_ip_address(3000, "127.0.0.3"));
    remote.send_to(metadata, address_to_send);
    for (int package {0}; package < metadata.packages_number; package++) {
      message = SetInfo(file.GetData(), package);
      remote.send_to(file.GetMappedMem() + (package * MESSAGE_SIZE),
                      address_to_send, sizeof(message));
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
    std::cerr << "netcp: " << "introduzca solo 1 archivo que copiar" << "\n";
  }
  catch (...) {
    std::cout << "Error desconocido\n";
    return 99;
  }
  return 0;
}


int receive() {
    try {
    Socket local(make_ip_address(2000, "127.0.0.1"));
    FileMetadata metadata;
    // Message m_recibido;
    sockaddr_in address_to_receive = make_ip_address(3000, "127.0.0.3");
    metadata = local.receive_metadata(address_to_receive);
    // Chivatos del metadata
    std::cout << "Datos de:" << metadata.filename.data()
              << "\nSeparado en: " << metadata.packages_number
              << "\nTamaño: " << metadata.file_size << std::endl;
    std::string string_filename(metadata.filename.data());
    // El archivo donde se guardará el mensaje recibido
    File file("salida.txt", metadata.file_info);
    std::string total_text;   //< Todo el contenido de todos los mensajes juntos
    for (int i {0}; i < metadata.packages_number - 1; i++) {
      // local.receive_from(address_to_receive, file.GetMappedMem(), file.GetFileSize());
      local.receive_from(address_to_receive,
                file.GetMappedMem() + (i * MESSAGE_SIZE), MESSAGE_SIZE);
    }
    if (metadata.file_size % MESSAGE_SIZE != 0) {
      local.receive_from(address_to_receive,
            file.GetMappedMem() + (metadata.packages_number - 1) * MESSAGE_SIZE,
                          metadata.file_size % MESSAGE_SIZE);
    } else {
      local.receive_from(address_to_receive,
            file.GetMappedMem() + (metadata.packages_number - 1) * MESSAGE_SIZE,
            MESSAGE_SIZE);
    }
    // file.SetData(total_text);
    std::cout << "Archivo completo:\n" << total_text << std::endl;
  }
  catch(std::bad_alloc& e) {
    std::cerr << "netcp" << ": memoria insuficiente\n";
    return 1;
  }
  catch(std::system_error& e) {
    std::cerr << "netcp" << ": " << e.code() << '\n';
    return 2;
  }
  catch (...) {
    std::cout << "Error desconocido\n";
    return 99;
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
std::cout << std::endl;
std::cout << "Puerto: " << direction.sin_port << " (" << port << ")"
          << std::endl;
std::cout << "Direccion: " << direction.sin_addr.s_addr << " (" << ip_address
          << ")" << std::endl << std::endl;
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


Message SetInfo(const std::string& text, const int& package) {
  Message message;
  // Primer paquete
  if (package == 0) {
    for (int i {0}; i < MESSAGE_SIZE ; i++) {
      if ((size_t)i < text.size()) {
        message.data.at(i) = text.at(i);
      } else {
        // message.data.at(i) = '\0';
      }
    }
  } else {
  // Demás paquetes
    for (int i {0}; i < MESSAGE_SIZE; i++) {
      if ((size_t)(i + MESSAGE_SIZE * package) < text.size()) {
        message.data.at(i) = text.at(i + (MESSAGE_SIZE * package));
      } else if ((size_t)(i + MESSAGE_SIZE * package) == text.size()) {
        message.data.at(i) = '\0';
      }
    }
  }
  // message.data.at(MESSAGE_SIZE - 1) = '\0';
  return message;
}
