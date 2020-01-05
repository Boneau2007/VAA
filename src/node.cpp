

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

Node::Node() : id(0), ipAddress(""), port(0),  maxNeighbor(0), maxSend(0), believerEpsilon(0){}

Node::Node(const unsigned int id) : id(id), ipAddress(""), port(0),  maxNeighbor(0), maxSend(0), believerEpsilon(0){}

Node::Node(const unsigned int id, const string ipAddress, const unsigned int port)
    : id(id), ipAddress(ipAddress), port(port), maxNeighbor(0), maxSend(0), believerEpsilon(0){
}

Node::Node(const unsigned int ownId, const unsigned int initNodePort, const std::string nodeFile, 
            const unsigned int maxNeighbor, const unsigned int maxSend, const unsigned int believerEpsilon)
      : maxNeighbor(maxNeighbor), maxSend(maxSend), believerEpsilon(believerEpsilon){
    fileHandler = new FileHandler(nodeFile);
    // for(unsigned int i=0;i < fileHandler->getNodeList().size();i++){
    //         cout << fileHandler->getNodeList().at(i).toString() << endl;
    //     }
    Node ownNode = fileHandler->getNodeFromFile(ownId);
    id = ownNode.getId();
    ipAddress = ownNode.getIpAddress();
    port = ownNode.getPort();
    selectNeighbors();
    
    initNode = new Node(0, "127.0.0.1", initNodePort);
    startHandle();
}

Node::Node(const unsigned int ownId, const unsigned int initNodePort, const std::string nodeFile, 
            const std::string graphizFile, const unsigned int maxSend, const unsigned int believerEpsilon)
         : maxSend(maxSend), believerEpsilon(believerEpsilon){
    fileHandler = new FileHandler(nodeFile, graphizFile);
    Node ownNode = fileHandler->getNodeFromFile(ownId);
    id = ownNode.getId();
    ipAddress = ownNode.getIpAddress();
    port = ownNode.getPort();
    selectNeighbors();
    for(unsigned int i=0; i < neighbors.size(); i++){
        cout << "FROM: " << id <<" "<<neighbors.at(i).toString() << endl;
    }
    initNode = new Node(0, "127.0.0.1", initNodePort);
    startHandle();
}
Node::Node(const Node& node){
    maxSend = node.maxSend;
    believerEpsilon = node.believerEpsilon;
    id = node.id;
    *initNode = *node.initNode;
    fileHandler = node.fileHandler;
    neighbors = node.neighbors;
    ipAddress = node.ipAddress;
    port = node.port;
}

void Node::startHandle(){
    struct sockaddr_in address;
	int addrLenght = sizeof(address);
    vector<thread> threadPool;
    int listenFd = -1;
    initTcpSocket(listenFd, port);
    ostringstream ss;
    ss << "Has Id: " << id;
    Message message(id, MESSAGE_TYPE::APPLICATION, ss.str());
    threadPool.push_back(thread(&Node::executePingThread, this));

    for(unsigned int i=0; i< neighbors.size(); i++){
        threadPool.push_back(thread(&Node::executeSendMessageThread, this, message, neighbors.at(i)));
    }

    while(true){
        int worker = accept(listenFd,(struct sockaddr *)&address, (socklen_t *)&addrLenght);
        threadPool.push_back(thread(&Node::executeWorkerThread, this, worker));
    }

    for(unsigned int i=0;i < threadPool.size();i++){
        threadPool.at(i).join();
    }
}

void Node::selectNeighbors(){
    if(fileHandler->getGraphFile().empty()){
        srand (time(NULL));
        for(unsigned int i=0;i < maxNeighbor;i++){
            bool foundNeighbor = false;
            while(!foundNeighbor){
                unsigned int num = rand()%fileHandler->getNodeList().size(); 
                Node randNode = fileHandler->getNodeList().at(num);
                if(randNode.getId() != id && !hasNeighbor(randNode.getId())){ 
                    neighbors.push_back(randNode);
                    foundNeighbor = true;
                }
            }
        }
    }else{
        neighbors = fileHandler->readGraphviz(id);
    }   
}

void Node::sendOwnIdMessage(Message msg){ 
    MessageHandler handler(*this);
    for(unsigned int i=0; i< neighbors.size(); i++){
        handler.sendMessage(msg, neighbors.at(i));
    }
}
void Node::executeSendMessageThread(Message message, Node node){
    sleep(2);
    MessageHandler handler(*this);
    handler.sendMessage(message, node);
}
void Node::executeComHandleThread(string message) {
    MessageHandler handler(*this, message);
    handler.handleIncommingMessage();
}

void Node::executeWorkerThread(int socketFd) {
    char buff[256] = {'\0'};
    read(socketFd, buff, 256);
    string msg(buff);
    thread t(&Node::executeComHandleThread ,this, msg);
    close(socketFd);
    t.join();
}

void Node::executePingThread() {
    MessageHandler handler(*this);
    while (true){
        sleep(2);
        //handler.sendPing();
    }
}

bool Node::hasNeighbor(const unsigned int id){
    for(unsigned int i=0;i < neighbors.size();i++){
        if(neighbors.at(i).getId() == id){
            return true;
        }
    }
    return false;
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
