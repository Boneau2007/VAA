
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <regex>
#include <cstdlib>
#include <chrono>
#include <stdexcept>
#include "node.hpp"
extern "C" { 
#include "linuxsocket.h"
}
#define BUFF_SIZE 256
const int MAX_NEIGBOHR_NODES = 3;

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

    }
}

int handleOutgoingMessages(int socket){
    
}

void connectWithNeighbors(){
	//TODO: Implement Websocket connection with the neigbohrs. OR Should i just make simple http requests?
}

int main(int argc, char* argv[]){
    const string NODELINE_REGEX = "(([0-9]*)\s(.*):([0-9]*)(\n|))";
    if(argc != 3){
        cout << "usage: ./" << argv[0]  << " [FILE] [ID]" << endl;
        cout << "e.g: ./"<< argv[0] << " nodes.txt 4" << endl;
        cout << "[ID]: must be contained in File" << endl;
        exit(EXIT_FAILURE);
    }
    int tcpListener;
    map<unsigned int,Uebung1::Node> nodeListInFile;
    string fileName(argv[1]);
    unsigned int ownId = atoi(argv[2]);
    ifstream fileStream;
    fileStream.open(fileName, ios::out);
    
    if(!fileStream.is_open()){
        cout << "Error: Can't open file" << endl;
        exit(EXIT_FAILURE);
    }else{
        string line;
        cmatch matches;
        const regex isNodeLine(NODELINE_REGEX);
        for(int i=0; getline(fileStream, line); i++){
            if(line.empty()){
                cout << "Skip empty line";
                break;
            }
            if(regex_match(line, matches, isNodeLine)){
                unsigned int id = atoi(matches.str(1).c_str());
                string ipAddress(matches[2]);
                unsigned int port = atoi(matches.str(3).c_str());
                nodeListInFile.insert(id,Uebung1::Node(id, ipAddress, port));
            }else{
                cout << "No matching Regex in line: " << line << endl;
                cout << "Please edit your file.";
                exit(EXIT_FAILURE);
            }
        }
        fileStream.close();
    }
    if((tcpListener = initTcpSocket(nodeList.at(ownId).getPort())) < 0){
        exit(tcpListener);
    }
    for( auto const& [key, value] : nodeList ){
        if(nodeList.at(ownId).getNeighbors.getSize() != MAX_NEIGBOHR_NODES && key != ownId){
            nodeList.at(ownId).addNeighbor(value);
        }
    }
    connectWithNeighbors();
    bool idSendToNeighbors = false;
    while(true){
        acceptSocket(&tcpListener, &tcpListener);
        handleIncommingMessages(tcpListener);
        if(!idSendToNeighbors){
            //TODO: Sende an die nachbarn
            idSendToNeighbors = true;
        }
    }
    exit(EXIT_SUCCESS);
}
