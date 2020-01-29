

#include <sstream>
#include <regex>
#include <thread>         // std::thread
#include <queue>         // std::queue
#include <cstring>      // memset
#include <unistd.h>     // close()
#include <algorithm>    // begin(), end(), find()
#include <utility>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fstream>
#include "node.hpp"

using namespace std;
using namespace Uebung1;
using namespace chrono;

Node::Node() :  id(0), ipAddress(""), port(0), recvRumors(0), hasSend(false){}

Node::Node(const unsigned int id, const string& ipAddress, const unsigned int port)
    :   id(id), ipAddress(ipAddress), port(port), recvRumors(0), hasSend(false){
}

Node::Node(const unsigned int id, const std::string& configFile, const bool isGraphviz) : id(id),config(configFile){
    initNode = new Node(0,config.getInitIpAddress(), config.getInitPort());
    fileHandler = new FileHandler(config.getNodeFileName());
    doubleCounting = new DoubleCounting(this);
    messageHandler = new MessageHandler(this);
    fileHandler->readNodes(config.getMaxNumOfNodes());
    for(auto & node : fileHandler->getNodeList()){
        if(node.getId() == id){
            ipAddress = node.getIpAddress();
            port = node.getPort();
            break;
        }
    }
    if(isGraphviz){
        neighbors = fileHandler->readGraphviz(id, config.getGraphvizFileName());
    }else{
        selectNeighbors();
    }
    selectTime();
    startHandle();
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
}

void Node::executeSendMessageThread(Message message, const Node& node){
    sleep(2);
    sendMessageToNode(std::move(message), node);
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

void Node::sendToNeighbors(const Message& msg){
    for(const auto & neighbor : neighbors){
        sendMessageToNode(msg, neighbor);
    }
}

void Node::sendToSuperNode(const Message& msg){
    sendMessageToNode(msg, *initNode);
}

void Node::sendToNeighborsExceptSource(const Message& msg){
    for(const auto & neighbor : neighbors){
        if(neighbor.getId() != msg.getOriginId()){
            sendMessageToNode(msg , neighbor);
        }
    }
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
    cout << ss.str();
    messageHandler->handleIncommingMessage(&msg);
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

bool Node::hasNeighbor(const vector<Node>& nodes, const unsigned int thisId){
    for(auto & neighbor : nodes){
        if(neighbor.getId() == thisId){
            return true;
        }
    }
    return false;
}

void Node::selectNeighbors(){
    srand (time(nullptr));
    for(unsigned int i=0;i < fileHandler->getNodeList().size(); i++){
        bool foundNeighbor = false;
        while(!foundNeighbor){
            unsigned int num = rand()%fileHandler->getNodeList().size();
            Node randNode = fileHandler->getNodeList().at(num);
            if(randNode.getId() != id && !hasNeighbor(neighbors, randNode.getId())){
                neighbors.push_back(randNode);
                foundNeighbor = true;
            }
        }
    }
}


void Node::selectTime(){
    vector<time_t> timeList;
    ifstream fileStream;
    string line;
    fileStream.open(config.getTimesFile(), ios::out);
    if(!fileStream.is_open()){
        throw  runtime_error("Can't open file");
    }else{
        if(fileStream.peek() == ifstream::traits_type::eof()){
            throw runtime_error("ERROR: Empty file ");
        }
        for(unsigned int i=0; getline(fileStream, line); i++){
            if(line.empty()){
                continue;
            }
            timeList.push_back(stoi(line.c_str()));
        }
    }
    unsigned int num = rand()%timeList.size();
    preferedTime = timeList.at(num);
}

string Node::toString() const {
    ostringstream ss;
    ss << id << " " << ipAddress << " " << port;
    return ss.str();
}
