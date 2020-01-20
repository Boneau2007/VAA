

#include <sstream>
#include <thread>         // std::thread
#include <future>         // std::future
#include <queue>         // std::queue
#include <cstring>      // memset
#include <unistd.h>     // close()
#include <algorithm>    // begin(), end(), find()
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "include/node.hpp"
#include "include/messageHandler.hpp"

using namespace std;
using namespace Uebung1;

Node::Node() :  id(0), ipAddress(""), port(0), initNodePort(0), 
                maxSend(0), believerEpsilon(0), recvRumors(0), 
                hasSend(false), virtualParentId(0){}

Node::Node(const unsigned int id) : id(id), ipAddress(""), port(0), initNodePort(0), 
                                    maxSend(0), believerEpsilon(0), recvRumors(0), 
                                    hasSend(false), virtualParentId(0){}

Node::Node(const unsigned int id, const string ipAddress, const unsigned int port)
    :   id(id), ipAddress(ipAddress), port(port), initNodePort(0), maxSend(0), 
        believerEpsilon(0), recvRumors(0), hasSend(false), 
        virtualParentId(0){
}

Node::Node( const unsigned int id, const string ipAddress, const unsigned int port, const unsigned int initNodePort, 
            const vector<Node> neighbors, const unsigned int maxSend, const unsigned int believerEpsilon, 
            const time_t preferedTime, const unsigned maxStartNumber, const unsigned maxPhilosopherNumber)
    :   id(id), ipAddress(ipAddress), port(port),
        neighbors(neighbors), initNodePort(initNodePort), 
        maxSend(maxSend), believerEpsilon(believerEpsilon),
        recvRumors(0), hasSend(false), winner(false), initiator(false), virtualParentId(0),
        preferedTime(preferedTime), maxStartNumber(maxStartNumber), maxPhilosopherNumber(maxPhilosopherNumber), done(false){
    startHandle();
}

Node::Node(const Node& node){
    id = node.id;
    ipAddress = node.ipAddress;
    port = node.port;
    maxSend = node.maxSend;
    believerEpsilon = node.believerEpsilon;
    neighbors = node.neighbors;
}

void Node::startHandle(){
    //  cout << "id " << id <<  " "<< ipAddress << " "<< port << " "<< initNodePort<<  endl;    
    struct sockaddr_in address;
	int addrLenght = sizeof(address);
    vector<thread> threadPool;
    int listenFd = -1;
    initTcpSocket(listenFd, port);
    ostringstream ss;
    ss << "Node Id: " << id;

    //Send Id to neighbors
    Message message(id, MESSAGE_TYPE::APPLICATION, ss.str());
    for(unsigned int i=0; i< neighbors.size(); i++){
        threadPool.push_back(thread(&Node::executeSendMessageThread, this, message, neighbors.at(i)));
    }

    //Listen on socket and accept incomming connections
    while(true){
        try{
            int worker = accept(listenFd,(struct sockaddr *)&address, (socklen_t *)&addrLenght);
            threadPool.push_back(thread(&Node::executeWorkerThread, this, worker));
        }catch(exception& e){
            cout << e.what() << endl;
        }
    }

    //Wait for all threads to be finished
    for(unsigned int i=0;i < threadPool.size();i++){
        threadPool.at(i).join();
    }
}

void Node::sendOwnIdMessage(Message msg){
    MessageHandler handler(this);
    for(unsigned int i=0; i< neighbors.size(); i++){
        handler.sendMessage(msg, neighbors.at(i));
    }
}

void Node::executeSendMessageThread(Message message, Node node){
    sleep(2);
    MessageHandler handler(this);
    handler.sendMessage(message, node);
}

void Node::executeWorkerThread(int socketFd) {
    char buff[256] = {'\0'};
    read(socketFd, buff, 256);
    string msg(buff);
    MessageHandler handler(this, msg);
    handler.handleIncommingMessage();
    close(socketFd);
}

/*
 * @function	initTcpSocket
 * @abstract	Initialize Tcp-Socket
 * @discuss 	This function initializes the Socket for the Tcp communication
 * @param		port		Portnumber
 * @result		Results an EXIT_FAILURE or EXIT_SUCCESS
 */
void Node::initTcpSocket(int& socketFd, unsigned int port){
    struct sockaddr_in serverAddress;
    int yes = 1;
	if ((socketFd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        throw  runtime_error("Error: on socket create\n"); 
	}
    if (setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
        throw runtime_error("Error: setsockopt\n"); 
    }
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_port = htons(port);
	if (bind(socketFd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0){
        throw runtime_error("Error: on socket bind\n"); 
	}
    if (listen(socketFd, 0) < 0){
        throw runtime_error("Error: on socket listen\n"); 
	}
}

string Node::toString() const {
    ostringstream ss;
    ss  << "ID: "<< id <<  " IP: " << ipAddress << " Port: " << port;
    return ss.str();
}
