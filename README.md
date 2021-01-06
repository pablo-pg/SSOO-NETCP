# SSOO-NETCP

Programa desarrollado por Pablo Pérez González (alu0101318318@ull.edu.es).
## **Descripción**
Este programa envía y recibe ficheros. Si se quiere enviar o recibir, se introduce como parámetro en la ejecución.

## **Compilación**
Para compilar hay un makefile. Opciones:

* `make netcp`: Crea el ejecutable.
* `make run`: Crea el ejecutable y ejecuta la recepción.
* `make withfile`: Crea el ejecutables y ejecuta el envío de un archivo llamado `prueba.txt`
* `make clear`: Limpia todos los ejecutables.
* `make clean`: Limpia todos los ejecutables.

## **Ejecución**
El programa envia por pantalla flags de casi todo, direccion, puerto, mensaje enviado por paquete, mensaje recibido por paquete, etc...

En la ejecución se debe indicar por terminal la operación a hacer y en el caso de ayuda un fichero a copiar.

Existe el comando `--help` y `-h` que dan una breve explicaión.

Formas de ejecutarse:
```
./netcp receive
./netcp send [FICHERO].[EXTENSIÓN]
./netcp -h
./netcp --help
```
