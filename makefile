# Makefile for program

## Compiler configuration
CC = gcc
CFLAGS = -Wall -pedantic
OBJ = *.o

# Artefacts
all: localNode *.o

localNode:
${CC} ${CFLAGS} main.c -o localNode linuxsocket.o

linuxsocket.o:
${CC} ${CFLAGS} -c linuxsocket.c

main.o:
${CC} ${CFLAGS} -c main.c

# Cleans the directory

*.o:
	@ echo "\nLösche dateien mit Endung ${OBJ}"
	rm -f ${OBJ}

clean:
	@ echo "\nLösche alle Artefakte"
  rm -f ${OBJ} server serverT client
