 
#
# Universidad de La Laguna
# Escuela Superior de Ingeniería y Tecnología
# Sistemas Operativos 2020/21
#
# Autor: Pablo Pérez González
# Fecha: Diciembre de 2020
# e-mail: alu0101318318@ull.edu.es
# 
# Makefile para la Práctica de C++: Netcp.
#


CC=g++
CFLAGS=-O0 -g -Wall
DEPS = socket.h
OBJ = socket.o file.o netcp.o main_functions.o

%.o: %.cc $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)


netcp: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^


run: clean netcp
	clear
	./netcp receive

withfile: clean netcp
	./netcp send prueba.txt

clean:
	rm -f hellomake *.o netcp salida.txt

clear: clean
