 
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
OBJ = socket.o main.o

%.o: %.cc $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

netcp: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^


run: clean netcp
	clear
	./netcp

clean:
	rm -f hellomake *.o netcp

clear: clean

# withfile: clean netcp
# 	clear
# 	./dfa2g input.dfa output.gra