 
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
CFLAGS=-O0 -g -Wall -Wno-pointer-arith
DEPS = socket.h message.h file.h main_functions.h
LIBS = -pthread
OBJ = socket.o file.o netcp.o main_functions.o

%.o: %.cc $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)


netcp: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f hellomake *.o netcp temp_exit.txt

clear: clean
celar: clean
claer: clean
claen: clean
