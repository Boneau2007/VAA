

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
using namespace Graph;
using namespace Handler;
using namespace chrono;

Node::Node() :  id(0), ipAddress(""), port(0), recvRumors(0), hasSend(false),
                config(""), preferedTime(0), fileHandler(nullptr), messageHandler(nullptr),
                coordinator(nullptr), initNode(nullptr), waitAck(false){}

Node::Node(const unsigned int id, string  ipAddress, const unsigned int port)
    :   id(id), ipAddress(std::move(ipAddress)), port(port), recvRumors(0), hasSend(false),
        config(""), preferedTime(0), fileHandler(nullptr), messageHandler(nullptr),
        coordinator(nullptr), initNode(nullptr), waitAck(false){
}

Node::Node( const unsigned int id, const std::string& configFile, const bool isGraphviz) : id(id), recvRumors(0),
            hasSend(false), config(configFile), preferedTime(0), coordinator(nullptr), waitAck(false){
    initNode = new Node(0,config.getInitIpAddress(), config.getInitPort());
    fileHandler = new FileHandler(config.getNodeFileName());
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
    threadPool.emplace_back(&Node::executeAccountAlgorithmThread, this);
    //Listen on socket and accept incomming connections
    while(true){
        try{
            int worker = accept(listenFd,(struct sockaddr *)&address, (socklen_t *)&addrLenght);
            threadPool.emplace_back(&Node::executeWorkerThread, this, worker);
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


void Node::executeAccountAlgorithmThread() {
    unsigned int randIndex = -1;
    srand(time(nullptr));
    while(true){
        // 1. Warte zufällige Zeit 0 - 3
        sleep(rand()%3);
        // 2. Wähle zufälligen Knoten im Netzwerk
        randIndex = rand()%fileHandler->getNodeList().size();
        while(fileHandler->getNodeList().at(randIndex).getId() == id){
            randIndex = rand()%fileHandler->getNodeList().size();
        }
        // 3. Bekomme ein Lock
        //TODO: make wating time nodeList size dependend
        acquireLock(system_clock::to_time_t(system_clock::now())+id, randIndex);
        criticalSection(randIndex);
        // 10. Gebe lock frei
        loseLock();
    }
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
    for(unsigned int i=0;i < config.getMaxRandNumber(); i++){
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

void Node::acquireLock(unsigned int waitTillTime, unsigned int randIndex) {
    bool sendObpl = true;
    Message lockMsg(id, APPLICATION, "request", system_clock::to_time_t(system_clock::now()));
    for(const auto& node : fileHandler->getNodeList()){
        sendMessageToNode(lockMsg, node);
    } // TODO: Ersetze durch Flooding Alg.
    messageHandler->getAccountHandler()->pushMessageToQueue(lockMsg);
    while(!(messageHandler->getAccountHandler()->isLowestTimestamp() &&
            messageHandler->getAccountHandler()->receivedReplyFromAll())){
        if(system_clock::to_time_t(system_clock::now()) >= waitTillTime && sendObpl){
            messageHandler->getGoldmanEdgeChasingHandler()->initiateOBPL(messageHandler->getAccountHandler()->getBlockingNodeIdList());
            sendObpl = false;
        }
    }
}
void Node::loseLock(){
    messageHandler->getAccountHandler()->removeFromMessageQueue(id);
    Message releaseMsg(id, APPLICATION, "release", system_clock::to_time_t(system_clock::now()));
    for(const auto& node : fileHandler->getNodeList()){
        sendMessageToNode(releaseMsg, node);
    }
}

void Node::criticalSection(unsigned int randIndex){
    // 4. Wähle zufälligen Prozentsatz
    messageHandler->getAccountHandler()->setPercent(rand()%100);
    // 5. Sende den eigenen Kontostand und Prozentsatz
    ostringstream ss;
    ss << "send balance" << messageHandler->getAccountHandler()->getBalanceAmount() << ";" << messageHandler->getAccountHandler()->getPercent() ;
    Message message(id,APPLICATION,ss.str());
    sendMessageToNode(message, fileHandler->getNodeList().at(randIndex));
    // 6. Request Balance
    Message requestMessage(id,APPLICATION,"balance request");
    sendMessageToNode(requestMessage, fileHandler->getNodeList().at(randIndex));
    waitAck = true;
    // 9. Warte auf ACK und gib waitAck frei
    while(waitAck){
        sleep(1);
    }
}