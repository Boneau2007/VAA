

#include <sstream>
#include <regex>
#include <thread>         // std::thread
#include <future>         // std::future
#include <queue>         // std::queue
#include <cstring>      // memset
#include <unistd.h>     // close()
#include <algorithm>    // begin(), end(), find()
#include <utility>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "include/node.hpp"
#include "include/messageHandler.hpp"

using namespace std;
using namespace Uebung1;
using namespace chrono;

Node::Node() :  id(0), ipAddress(""), port(0), initNodePort(0), 
                maxSend(0), believerEpsilon(0), recvRumors(0), 
                hasSend(false){}

Node::Node(const unsigned int id) : id(id), ipAddress(""), port(0), initNodePort(0), 
                                    maxSend(0), believerEpsilon(0), recvRumors(0), 
                                    hasSend(false){}

Node::Node(const unsigned int id, const string ipAddress, const unsigned int port)
    :   id(id), ipAddress(ipAddress), port(port), initNodePort(0), maxSend(0), 
        believerEpsilon(0), recvRumors(0), hasSend(false){
}

Node::Node( const unsigned int id, const string ipAddress, const unsigned int port, const unsigned int initNodePort, 
            const vector<Node> neighbors, const unsigned int maxSend, const unsigned int believerEpsilon, 
            const time_t preferedTime,const unsigned int maxStartNumber, const unsigned maxPhilosopherNumber)
    :   id(id), ipAddress(ipAddress), port(port), neighbors(neighbors), initNodePort(initNodePort),
        maxSend(maxSend), believerEpsilon(believerEpsilon), recvRumors(0), hasSend(false),
        preferedTime(preferedTime), maxStartNumber(maxStartNumber),maxPhilosopherNumber(maxPhilosopherNumber), handler(this){
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
    struct sockaddr_in address{};
	int addrLenght = sizeof(address);
    vector<thread> threadPool;
    int listenFd = -1;
    initTcpSocket(listenFd);
    ostringstream ss;
    ss << "Node Id: " << id;

    //Send Id to neighbors
    Message message(id, MESSAGE_TYPE::APPLICATION, ss.str());
    for(auto & neighbor : neighbors){
        threadPool.emplace_back(&Node::executeSendMessageThread, this, message, neighbor);
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

/*
 * This function creates communication sockets and writes data to the host
 */
void Node::sendMessageToNode(Message message, const Node& targetNode){
    int sock;
    struct sockaddr_in server{};
    try{
        if((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1){
            throw runtime_error("ERROR: Unable to create socket");
        }
        server.sin_family = AF_INET;
        server.sin_port = htons(targetNode.getPort());
        if(inet_pton(AF_INET, targetNode.getIpAddress().c_str(),&server.sin_addr) == -1){
            throw runtime_error( "ERROR: Invalid Addess");
        }
        if(connect(sock, (struct sockaddr*)&server, sizeof(server)) == -1){
            throw runtime_error("ERROR: Unable to connect");
        }
        time_t time = system_clock::to_time_t(system_clock::now());
        if(write(sock, message.toString().c_str(), message.toString().size()) == -1){
            throw runtime_error("ERROR: Unable to write");
        }
        close(sock);
        string timeS(asctime(localtime(&time)));
        timeS.pop_back();
        ostringstream ss;
        ss << "[ID:" << id << "] SEND: " << message.toString() << " [" << timeS << "]" << endl;
        //std::cout << ss.str();
    }
    catch(const exception& e){
        cerr << e.what() << endl;
    }
}

void Node::sendToNeighbors(Message msg){
    for(const auto & neighbor : neighbors){
        sendMessageToNode(msg, neighbor);
    }
}

void Node::executeSendMessageThread(Message message, const Node& node){
    sleep(2);
    sendMessageToNode(std::move(message), node);
}

void Node::executeWorkerThread(int socketFd) {
    ostringstream ss;
    char buff[256] = {'\0'};
    read(socketFd, buff, 256);
    time_t time = system_clock::to_time_t(system_clock::now());
    string timeS(asctime(localtime(&time)));
    timeS.pop_back();
    string msgS(buff);
    Message msg(msgS);
    ss << "[ID:"<< id << "] RECV: " << msg.toString() << " [" << timeS <<"]"  << endl;
    //cout << ss.str();
    handler.handleIncommingMessage(msg);
    close(socketFd);
}

/*
 * @function	initTcpSocket
 * @abstract	Initialize Tcp-Socket
 * @discuss 	This function initializes the Socket for the Tcp communication
 * @param		port		Portnumber
 * @result		Results an EXIT_FAILURE or EXIT_SUCCESS
 */
void Node::initTcpSocket(int& socketFd){
    struct sockaddr_in serverAddress{};
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

//void Node::addEchoNeighborMsg(string nodeEchoMsg){
//    stringstream ss;
//    ss << nodeEchoMsg;
//    char delemiter(';');
//    string line;
//    smatch matches;
//    const regex isNodeString("([0-9]+) ([0-9]+.[0-9]+.[0-9]+.[0-9]+) ([0-9]+)");
//    for(unsigned int i=0; getline(ss, line, delemiter); i++){
//        regex_match(line, matches, isNodeString);
//        Node node(atoi(matches.str(1).c_str()), matches.str(2), atoi(matches.str(3).c_str()));
//        echoNeighbors.push_back(node);
//    }
//}

string Node::toString() const {
    ostringstream ss;
    ss  << id <<  " " << ipAddress << " " << port;
    return ss.str();
}
