#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <regex>
#include <cstdlib>
#include <chrono>
#include <stdexcept>
#include <netdb.h> //C
#include <bits/stdc++.h> // C      
#include <unistd.h> // C
#include <fcntl.h> // C
#include "node.hpp"
#include "linuxsocket.h"
#define BUFF_SIZE 256
const string NODELINE_REGEX = "(([0-9]*)\\s(.*):([0-9]*)(\n|))";
const unsigned int END = 0;
const unsigned int UPGRADE = 1;
const unsigned int REMOTE_SHUTDOWN = 2;
const unsigned int REMOTE_SHUTDOWN_ALL = 3;

void setHostAddress(struct sockaddr_in* host_addr, string host, unsigned int port){
  struct hostent* host_info;
  unsigned long address;
  if((address = inet_addr(host.c_str())) != INADDR_NONE){
    memcpy((char *)&host_addr->sin_addr, &address, sizeof(address));
  }else{
    host_info = gethostbyname(host.c_str());
    if(host_info==NULL){
      printf("\nUnknown Server");
      return;
    }else{
      memcpy((char *)&host_addr->sin_addr, &host_info->h_name, host_info->h_length);
    }
  }
  host_addr->sin_family = AF_INET;
  host_addr->sin_port = htons(port);
}

/*
 * @function	initTcpSocket
 * @abstract	Initialize Tcp-Socket
 * @discuss 	This function initializes the Socket for the Tcp communication
 * @param		port		Portnumber
 * @result		Results an EXIT_FAILURE or EXIT_SUCCESS
 */
int initTcpSocket(int port){
	int tcpSocket;
	if ((tcpSocket = createSocket(AF_INET, SOCK_STREAM, 0)) < 0){
		return -1;
	}else{
		printf("Tcp-listener is now at fd : [%d]\n", tcpSocket);
	}if (bindSocket(&tcpSocket, INADDR_ANY, port) < 0){
		return -2;
	}else{
		printf("Tcp-listener is now bound\n");
	}

	if (listenSocket(&tcpSocket) < 0)
	{
		return -3;
	}
	else
	{
		printf("Tcp-listener is now in listen mode\n");
	}
	return tcpSocket;
}

int handleIncommingMessages(int socket){
	char* buff = (char*)calloc(BUFF_SIZE, sizeof(char));
    int messageLength = 0 ;
    if((messageLength = recv(socket, buff, BUFF_SIZE, 0)) == -1){
		switch (errno){
		case EWOULDBLOCK: fprintf(stderr,"\nOperation would block. Try again\n");
						  return EWOULDBLOCK;
		default: fprintf(stderr,"\nError, recv() failed.\n");
				 return -1;
		}
	}else{
        chrono::system_clock::time_point now = chrono::system_clock::now();
        time_t time = chrono::system_clock::to_time_t(now);
        cout << buff << " : " << asctime(gmtime(&time)) << endl;
        if(strncmp(buff,"end",3)){
            exit(EXIT_SUCCESS);
        }
        return EXIT_SUCCESS;
    }
}
void startNeighbors(const map<unsigned int, Uebung1::Node> nodeList, string programName, string fileName, unsigned int size, unsigned int currentId){
    stringstream stream;
    stream << "./" << programName << " " << fileName;
    for( auto const& [key, value] : nodeList ){
        if(key == currentId){
            continue;
        }
        stringstream tmpStream;
        tmpStream << stream.rdbuf() << " " << key << " " << size << "" << std::boolalpha << value.getInitiator();
        string tmpCommand(tmpStream.str());
        system(tmpCommand.c_str());
    }
}

map<unsigned int, Uebung1::Node> getNodesInFile(string path){
    map<unsigned int, Uebung1::Node> nodeList;
    ifstream fileStream;
    fileStream.open(path, ios::out);
    if(!fileStream.is_open()){
    cout << "Error: Can't open file" << endl;
    exit(EXIT_FAILURE);
    }else{
        string line;
        smatch matches;
        const regex isNodeLineRegex(NODELINE_REGEX);
        for(int i=0; getline(fileStream, line); i++){
            if(line.empty()){
                cout << "Skip empty line";
                break;
            }
            if(regex_match(line, matches, isNodeLineRegex)){
                unsigned int id = atoi(matches.str(1).c_str());
                string ipAddress(matches[2]);
                unsigned int port = atoi(matches.str(3).c_str());
                nodeList.insert(make_pair(id,Uebung1::Node(id, ipAddress, port)));
            }else{
                cout << "No matching Regex in line: " << line << endl;
                cout << "Please edit your file.";
                exit(EXIT_FAILURE);
            }
        }
        fileStream.close();
    }
    return nodeList;
}
void setNeighborCount(map<unsigned int,Uebung1::Node> nodeList, unsigned int count,unsigned int id){
        for( auto const& [key, value] : nodeList ){
            if(nodeList.at(id).getNeighbors().size() != count && key != id){
                nodeList.at(id).addNeighbor(value);
            }
        }
    }

int handleOutgoingMessages(int socket, sockaddr_in host_addr, string message){
	if (connect(socket, (struct sockaddr*) & host_addr, sizeof(host_addr)) == -1) {
		return EXIT_FAILURE;
	}
	write(socket, message.c_str(), message.size());
	closeSocket(&socket);
    return EXIT_SUCCESS;
}

void dialog(){
    cout << UPGRADE << ". Upgrade node to initiator" << endl;
    cout << REMOTE_SHUTDOWN << ".Terminate node" << endl;
    cout << REMOTE_SHUTDOWN_ALL << ". Terminate all nodes" << endl;
    cout << END << ". Close Programm" << endl;
    cout << "Input: ";
}

int getInt(){
    long value;
    fflush(stdin);
    fgets((char*)&value, sizeof(value)-1, stdin);
    fflush(stdin);
    value = atoi((char*)&value);
    return value;
}

int main(int argc, char* argv[]){
    struct sockaddr_in host;
    struct timeval time;
    time.tv_sec = 0;
    time.tv_usec = 10;
    int err;
    bool idSendToNeighbors = false;
	int tcpListener;
	int worker;
    const string programName(argv[0]);
    const string fileName(argv[1]);
    const unsigned int ownId = atoi(argv[2]);
    const unsigned int neighborCount = atoi(argv[3]);
    string initiator(argv[4]);

    if(argc != 5){
        cout << "usage: ./" << programName  << " [FILE] [ID] [NEIGHBOR_SIZE] [INITIATOR(true|false)]" << endl;
        cout << "e.g: ./"<< programName << " nodes.txt 4 3 true" << endl;
        cout << "[FILE]: must be accessible" << endl;
        cout << "[ID]: must be contained in File" << endl;
        cout << "[NEIGHBOR_SIZE]: must be lower than node count in file" << endl;
        cout << "[INITIATOR]: must be true or false" << endl;
        exit(EXIT_FAILURE);
    }
    transform(initiator.begin(),initiator.end(),initiator.begin(), ::tolower);
    map<unsigned int,Uebung1::Node> nodeList = getNodesInFile(fileName);

    if(nodeList.size() <= neighborCount){
        cout << "Not enough Nodes in file for Network : Is" << nodeList.size() << " Min: "<< neighborCount+1 << endl;
        exit(EXIT_FAILURE);
    }
    if((tcpListener = initTcpSocket(nodeList.at(ownId).getPort())) < 0){
        exit(tcpListener);
    }
    setsockopt(tcpListener, SOL_SOCKET, SO_RCVTIMEO, (const char*)&time, sizeof(time));
    setNeighborCount(nodeList, neighborCount, ownId);
    if(initiator.compare("true")){
        nodeList.at(ownId).setInitiator(true);
        startNeighbors(nodeList, programName, fileName, neighborCount, ownId);
        sleep(1);
        dialog();
    }
    fd_set fdset;
	fcntl(tcpListener, F_SETFL, O_NONBLOCK);
	
    while(true){
        FD_ZERO(&fdset);
		FD_CLR(tcpListener, &fdset);
		FD_CLR(worker, &fdset);
		FD_CLR(STDIN_FILENO, &fdset);
		FD_SET(tcpListener, &fdset);
		FD_SET(worker, &fdset);
		FD_SET(STDIN_FILENO, &fdset);
interrupt:
		if ((err = select(FD_SETSIZE, &fdset, NULL, NULL, NULL) )< 0) {
			if (err < 0) {
        		switch (errno) {
        		case EINTR:     goto interrupt;
        		case EAGAIN:    break; /* empty rx queue */
        		case ETIMEDOUT: break; /* recv timeout */
        		case ENOTCONN:  break; /* not connected yet */
        		default:        printf("Connetction lost [%d]", err);
        		}
			}
		}
        if(nodeList.at(ownId).getInitiator()){
            if(FD_ISSET(STDIN_FILENO, &fdset)){
                switch(getInt()){
                    case END: exit(EXIT_SUCCESS);
                              break;
                    case UPGRADE: //handleOutgoingMessages();
                                  break;
                    case REMOTE_SHUTDOWN: //handleOutgoingMessages();
                                          break;
                    case REMOTE_SHUTDOWN_ALL: //handleOutgoingMessages();
                                              break;
                    default: dialog();
                }
            }
        }
		if (!idSendToNeighbors) {
			for (map<unsigned int, Uebung1::Node>::iterator it = nodeList.begin(); it != nodeList.end(); ++it) {
				setHostAddress(&host, it->second.getIpAddress(), it->second.getPort());
				handleOutgoingMessages(worker, host, to_string(it->second.getId()));
			}
			idSendToNeighbors = true;
		}
		//Connection happend
		if(FD_ISSET(tcpListener, &fdset)){
			if (worker == -1) {
				acceptSocket(&tcpListener, &worker);
			}
		}
		//IO-happens
		else {
			if (FD_ISSET(worker, &fdset)) {
				handleIncommingMessages(worker);
			}
		}
	}
	exit(EXIT_SUCCESS);
}
