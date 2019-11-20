# Makefile for program
## Compiler configuration
CPP = g++
CC = gcc
CFLAGS = -Wall -pedantic
CPPFLAGS = ${CFLAGS} -std=c++17
OBJ = *.o
INCLUDE_PATH=include
OBJECT_PATH=obj/
BIN_PATH=bin/
SRC_PATH=src/

# Artefacts
all: makeBuild build

makeBuild:
	rm -Rf ${OBJECT_PATH}
	mkdir ${OBJECT_PATH}

build: linuxsocket.o node.o nodeApp.o
	${CPP} ${CPPFLAGS} -o nodeApp ${OBJECT_PATH}linuxsocket.o ${OBJECT_PATH}node.o ${OBJECT_PATH}nodeApp.o

nodeApp.o: 
	${CPP} ${CPPFLAGS} -c  ${SRC_PATH}nodeApp.cpp -I ${INCLUDE_PATH} -o ${OBJECT_PATH}nodeApp.o

node.o:
	${CPP} ${CPPFLAGS} -c  ${SRC_PATH}node.cpp -I ${INCLUDE_PATH} -o ${OBJECT_PATH}node.o

linuxsocket.o:
	${CC} ${CFLAGS} -c  ${SRC_PATH}linuxsocket.c -I ${INCLUDE_PATH} -o ${OBJECT_PATH}linuxsocket.o

# Cleans the directory

*.o:
	rm -f ${OBJ}

clean:
	rm -Rf ${OBJ} ${OBJECT_PATH} nodeApp
