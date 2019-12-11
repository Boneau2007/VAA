
#ifndef NODEAPP_HPP
#define NODEAPP_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <regex>
#include <cstdlib>
#include <chrono>
#include <stdexcept>
#include <algorithm>
#include <netdb.h> //C
#include <bits/stdc++.h> // C      
#include <unistd.h> // C
#include <fcntl.h> // C

#include "node.hpp"
#include "graph.hpp"
#include "linuxsocket.h"
#define BUFF_SIZE 256
#define MAX_WORKER 128

using namespace std;

Uebung1::Graph readGraphviz(string path);
void setHostAddress(struct sockaddr_in* host_addr, string host, unsigned int port);
int initTcpSocket(int port);
int handleIncommingMessages(int socket);
void startNodes(const map<unsigned int, Uebung1::Node> nodeList, string programName, string fileName, unsigned int size, unsigned int currentId);
map<unsigned int, Uebung1::Node> getNodesInFile(string path);
int handleOutgoingMessages(int socket, sockaddr_in host_addr, string message);
string menueDialog();
string controlMessageDialog();
void communicationHandle(map<unsigned int,Uebung1::Node> nodeList, unsigned int ownId);


#endif // NODEAPP_HPP