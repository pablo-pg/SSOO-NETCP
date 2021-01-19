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

/**
 * @brief Envía un archivo a una dirección conocida (env).
 * 
 * @param eptr si ocurren excepciones.
 * @param argv el nombre del fichero a enviar.
 * @param quit_tarea2 si debe cancelarse el envío.
 * @param pause_send si  debe pausarse el envío.
 * @param quit_app si debe cancelarse el envío y cerrar el programa.
 * @return int 0 si no hay ningún fallo.
 */
int send_file(std::exception_ptr& eptr, std::string argv,
              std::atomic<bool>& quit_tarea2, std::atomic<bool>& pause_send,
              std::atomic<bool>& quit_app, const uint32_t& direction,
              const in_port_t port) {
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
    /// Se establecen los metadatos del fichero.
    FileMetadata metadata;
    metadata = SetMetadata(file.GetData(), filename, file.GetMetaInfo(),
                           direction, port);
    sockaddr_in address_to_send = make_ip_address(
            std::atoi(getenv("NETCP_DEST_PORT")),     //< El puerto pasado a int
            getenv("NETCP_DEST_IP"));
    Socket remote(make_ip_address(0,
                  getenv("NETCP_IP")));
    remote.send_to(metadata, address_to_send);      //< Se envían los metadatos.
    /// SE ENVÍA LA INFORMACIÓN
    for (int package {0}; package < metadata.packages_number; package++) {
      if (quit_tarea2 || quit_app) {
        std::cout << "Envío abortado." << std::endl;
        return 0;
      } else {
std::this_thread::sleep_for(std::chrono::seconds(2));     //< Hago que espere 2s
        while (pause_send) {
          std::this_thread::yield();
        }
        remote.send_to(file.GetMappedMem() + (package * (MESSAGE_SIZE - overhead)),
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


/**
 * @brief Recibe un archivo desde una dirección conocida (env) y lo mueve a
 * la carpeta creada "folder".
 * 
 * @param eptr si ocurren excepciones.
 * @param folder el nombre del fichero a enviar.
 * @param quit_tarea3 si debe cancelarse el envío.
 * @param quit_app si debe cancelarse el envío y cerrar el programa.
 * @return int 0 si no hay ningún fallo.
 */
int receive_file(std::exception_ptr& eptr, std::string folder,
                 std::atomic<bool>& quit_tarea3, std::atomic<bool>& quit_app) {
  try {
    std::unordered_map<std::pair<uint32_t, in_port_t>, ReceptionTask, pair_hash>
        receiving_tasks;
    Socket local(make_ip_address(std::atoi(getenv("NETCP_DEST_PORT")),
                getenv("NETCP_DEST_IP")));
    while (!quit_app && !quit_tarea3) {
      /// Se establecen las direcciones  puertos.
      sockaddr_in address_to_receive = make_ip_address(
                  std::atoi(getenv("NETCP_PORT")), getenv("NETCP_IP"));
      FileMetadata metadata;
      if (!quit_app && !quit_tarea3) {
        /// Se reciben los metadatos.
        metadata = local.receive_metadata(address_to_receive);
      }
      if (errno != EINTR) {
        std::string string_filename(metadata.filename.data());
        // El archivo donde se guardará el mensaje recibido.
        File file("temp_exit.txt", metadata.file_info);
        for (int i {0}; i < metadata.packages_number - 1; i++) {
          if (quit_tarea3 || quit_app) {
            std::cout << "Recepción de mensaje abortada." << std::endl;
            return 0;
          } else {
            /// SE RECIBE EL MENSAJE Y SE MAPEA EN MEMORIA.
            local.receive_from(address_to_receive,
                  file.GetMappedMem() + (i * (MESSAGE_SIZE - overhead)), MESSAGE_SIZE);
            std::cout << "Paquete " << i+1 << "/" << metadata.packages_number
                      << " recibido." << std::endl;
          }
        }    /// Se escribe en memoria el último paquete.
        if ((metadata.file_size + overhead) % MESSAGE_SIZE != 0) {
          local.receive_from(address_to_receive,
            file.GetMappedMem() + ((metadata.packages_number - 1) * (MESSAGE_SIZE - overhead)),
            (metadata.file_size + metadata.packages_number * overhead) % MESSAGE_SIZE);
          std::cout << "Paquete pequeño " << metadata.packages_number << "/"
                    << metadata.packages_number << " recibido." << std::endl;
        } else {
          local.receive_from(address_to_receive,
            file.GetMappedMem() + ((metadata.packages_number - 1) * (MESSAGE_SIZE - overhead)),
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


/// CONTROLADOR DE SEÑALES

void signal_handler(const sigset_t& set) {
  extern std::atomic_bool quit_app;
  while (true) {
    int signum;
    sigwait(&set, &signum);
    if (signum == SIGINT || signum == SIGHUP || signum == SIGTERM) {
      std::cout << "\nHa llegado una señal crítica\n";
      quit_app = true;
      exit(1);
    }
  }
}

void usr1_funct(int signal) {
  const char* message = "¡Algo ha ido mal! Señal SIGUSR1 recibida.\n";
  write(STDOUT_FILENO, message, strlen(message));
}

/// FUNCIONES SECUNDARIAS

/**
 * @brief Se crea una dirección válida donde serán enviados/recibidos ficheros.
 * 
 * @param port es el puerto a conectarse.
 * @param ip_address es la dirección IP a conectarse,
 * @return sockaddr_in es la estructura válida para el envío de mensajes.
 */
sockaddr_in make_ip_address(int port, const std::string& ip_address) {
  sockaddr_in direction{};
  direction.sin_family = AF_INET;
  direction.sin_port = htons(port);
  if (ip_address.empty()) {
    direction.sin_addr.s_addr = htonl(INADDR_ANY);
  } else {
    inet_aton(ip_address.c_str(), &direction.sin_addr);
  }
  if (port > 65525 || port < 0) {
    throw std::system_error(errno, std::system_category(),
                            "Puerto fuera de rango: " + port);
  }
  return direction;
}


/**
 * @brief Se establecen los metadatos del fichero.
 * 
 * @param text es el contenido del fichero.
 * @param filename es el nombre del fichero
 * @param meta_info es el conjunto de datos (permisos, usuarios, tamaño,..).
 * @return FileMetadata es la estructura con todos los metadatos.
 */
FileMetadata SetMetadata(const std::string& text, const std::string& filename,
                          const struct stat& meta_info, const uint32_t& dir,
                          const in_port_t& the_port) {
  FileMetadata metadata;
  for (size_t i{0}; i < filename.size(); i++) {
    metadata.filename.at(i) = filename.at(i);
  }
  metadata.direction = dir;
  metadata.port = the_port;
  metadata.filename.at(filename.size()) = '\0';
  metadata.file_size = text.size();
  metadata.packages_number = metadata.calculate_message_num(metadata.file_size);
  metadata.file_info = meta_info;
  return metadata;
}

/**
 * @brief Se mueve el fichero recibido a la ubicación deseada.
 * 
 * @param file_name Nombre que se quiere poner al fichero.
 * @param folder_name Nombre de la carpeta donde se va a guardar
 */
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
  char cmd[200];
  // std::cout << cmd_str << std::endl;
  std::strcpy(cmd, cmd_str.c_str());
  system(cmd);
}


void make_send(std::exception_ptr& eptr, std::string argv,
              std::atomic<bool>& quit_tarea2, std::atomic<bool>& pause_send,
              std::atomic<bool>& quit_app, const uint32_t& direction,
              const in_port_t port) {
  std::thread send_thread;
  send_thread = std::thread(send_file, std::ref(eptr), argv,
                           std::ref(quit_tarea2), std::ref(pause_send),
                           std::ref(quit_app), direction, port);
  send_thread.join();
  std::cout << "ENVIADO" << std::endl;
  // sent = true;
}

void make_receive(std::exception_ptr& eptr, std::string folder,
              std::atomic<bool>& quit_tarea3, std::atomic<bool>& quit_app) {
  std::thread receive_thread;
  Socket local(make_ip_address(std::atoi(getenv("NETCP_DEST_PORT")),
                  getenv("NETCP_DEST_IP")));
  receive_thread = std::thread(receive_file, std::ref(eptr), folder,
              std::ref(quit_tarea3), std::ref(quit_app));
  receive_thread.join();
}
