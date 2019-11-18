#ifdef __cplusplus
extern "C" {
#endif
#ifndef SOCKETS_H
#define SOCKETS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>

int createSocket(int family, int type, int protocol);
int listenSocket(int* socket);
int bindSocket(int* socket, unsigned int address, int port);
int acceptSocket(int* listener, int* worker);
int closeSocket(int* socket);
    
#endif // SOCKETS_H
#ifdef __cplusplus
}
#endif