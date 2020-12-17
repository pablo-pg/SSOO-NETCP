# SSOO-NETCP

## Compilación
Para compilar hay un makefile. Opciones:

* `make netcp_send`: Crea el ejecutable referente al envío.
* `make netcp_receive`: Crea el ejecutable referente a la recepción.
* `make all`: Crea los ejecutables necesarios para enviar y recibir mensajes.
* `make run`: Crea los ejecutables necesarios para enviar y recibir mensajes y ejecuta la recepción.
* `make clear`: Limpia todos los ejecutables.

## Ejecución
Ambos programas envian por pantalla flags de casi todo, direccion, puerto, mensaje enviado por paquete, mensaje recibido por paquete, etc...

En la ejecución se debe indicar por terminal un fichero a copiar.
