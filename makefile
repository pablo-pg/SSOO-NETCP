 
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
OBJ = socket.o file.o netcp_send.o 
OBJ2 = netcp_receive.o file.o socket.o

%.o: %.cc $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)


netcp_send: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

netcp_receive: $(OBJ2)
	$(CC) $(CFLAGS) -o $@ $^


all: clean netcp_receive netcp_send

run: clean netcp_send netcp_receive
	clear
	./netcp_receive

clean:
	rm -f hellomake *.o netcp_receive netcp_send

clear: clean

# withfile: clean netcp
# 	clear
# 	./dfa2g input.dfa output.gra