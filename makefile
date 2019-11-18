# Makefile for program
## Compiler configuration
CPP = g++
CC = gcc
CFLAGS = -Wall -pedantic -std=c++17
OBJ = *.o

# Artefacts
all: nodeApp *.o

nodeApp: linuxsocket.o node.o nodeApp.o
	${CPP} ${CFLAGS} -o nodeApp linuxsocket.o node.o nodeApp.o

nodeApp.o: 
	${CPP} ${CFLAGS} -c nodeApp.cpp

node.o:
	${CPP} ${CFLAGS} -c node.cpp 

linuxsocket.o:
	${CC} -Wall -pedantic -c linuxsocket.c 

# Cleans the directory

*.o:
	rm -f ${OBJ}

clean:
	rm -f ${OBJ} server serverT client
