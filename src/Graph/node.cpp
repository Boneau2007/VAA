

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
#include "Include/node.hpp"

using namespace std;
using namespace Graph;
using namespace Handler;
using namespace chrono;

Graph::Node::Node() :  id(0), ipAddress(""), port(0), recvRumors(0), hasSend(false), preferedTime(0), fileHandler(nullptr), messageHandler(nullptr),
                coordinator(nullptr), initNode(nullptr), waitAck(false){}

Graph::Node::Node(const unsigned int id, string  ipAddress, const unsigned int port) {
    this->id = id;
    this->ipAddress = std::move(ipAddress);
    this->port = port;
}

Graph::Node::Node( const unsigned int id, const std::string& configFile, const bool isGraphviz) : config(configFile){
    this->id = id;
    initNode = new Node(0,config.getInitIpAddress(), config.getInitPort());
    fileHandler = new Handler::FileHandler(config.getNodeFileName());
    messageHandler = new Handler::MessageHandler(this);
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


void Graph::Node::startHandle(){
    struct sockaddr_in address{};
	int addrLenght = sizeof(address);
    vector<thread> threadPool;
    int listenFd = -1;
    initTcpSocket(listenFd);
    ostringstream ss;
    ss << "Node Id: " << id;

    //Send Id to neighbors
    Messages::IMessage* message = new Messages::Message(id, Messages::MESSAGE_TYPE::APPLICATION, ss.str());
    for(auto & neighbor : neighbors){
        threadPool.emplace_back(&Node::executeSendMessageThread, this, message, neighbor);
    }
    sleep(3);
    // 3.Uebung
    threadPool.emplace_back(&Node::executeAccountAlgorithmThread, this);

    while(true){
        try{
            int worker = accept(listenFd,(struct sockaddr *)&address, (socklen_t *)&addrLenght);
            threadPool.emplace_back(&Node::executeWorkerThread, this, worker);
        }catch(exception& e){
            cout << e.what() << endl;
        }
    }
}

void Graph::Node::executeSendMessageThread(Messages::IMessage* message, const Node& node){
    sleep(2);
    sendMessageToNode(message, node);
}

/*
 * This function creates communication sockets and writes data to the host
 */
void Graph::Node::sendMessageToNode(Messages::IMessage* message, const Node& targetNode){
    int sock;
    struct sockaddr_in server{};
    string command = message->getCommand();
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
        //time_t time = system_clock::to_time_t(system_clock::now());
        //TODO: Wenn Nachricht Command enthält bestimmte nachrichten, dann ist Message
        if( command.find("Node Id: ") != string::npos ||
            command.find("end") != string::npos ||
            command.find("election") != string::npos ||
            command.find("initiator") != string::npos||
            command.find("rumor") != string::npos){
            Messages::Message* msg = dynamic_cast<Messages::Message*>(message);
            if(write(sock, msg->toString().c_str(), msg->toString().size()) == -1){
                throw runtime_error("ERROR: Unable to write");
            }
        }
        close(sock);
    }
    catch(const exception& e){
        cerr << e.what() << endl;
    }
}

void Graph::Node::sendToNeighbors(Messages::IMessage* msg){
    for(const auto & neighbor : neighbors){
        sendMessageToNode(msg, neighbor);
    }
}

void Graph::Node::sendToNeighborsExceptSource(Messages::IMessage* msg){
    for(const auto & neighbor : neighbors){
        if(neighbor.getId() != msg->getOriginId()){
            sendMessageToNode(msg , neighbor);
        }
    }
}

void Graph::Node::executeWorkerThread(int socketFd) {
    char buff[256] = {'\0'};
    read(socketFd, buff, 256);
    messageHandler->handleIncommingMessage(string(buff), system_clock::to_time_t(system_clock::now()));
    close(socketFd);
}


void Graph::Node::executeAccountAlgorithmThread() {
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
void Graph::Node::initTcpSocket(int& socketFd){
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

bool Graph::Node::hasNeighbor(const vector<Graph::Node>& nodes, const unsigned int thisId){
    for(auto & neighbor : nodes){
        if(neighbor.getId() == thisId){
            return true;
        }
    }
    return false;
}

void Graph::Node::selectNeighbors(){
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


void Graph::Node::selectTime(){
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

void Graph::Node::acquireLock(unsigned int waitTillTime, unsigned int randIndex) {
    bool sendObpl = true;
    Messages::IMessage* lockMsg = new Messages::AccountMessage(id, APPLICATION, "request", system_clock::to_time_t(system_clock::now()));
    //for(const auto& node : fileHandler->getNodeList()){
    //    sendMessageToNode(lockMsg, node);
    //} // TODO: Ersetze durch Flooding Alg.
    //messageHandler->getAccountHandler()->pushMessageToQueue(lockMsg);
    //while(!(messageHandler->getAccountHandler()->isLowestTimestamp() &&
    //        messageHandler->getAccountHandler()->receivedReplyFromAll())){
    //    if(system_clock::to_time_t(system_clock::now()) >= waitTillTime && sendObpl){
    //        messageHandler->getGoldmanEdgeChasingHandler()->initiateOBPL(messageHandler->getAccountHandler()->getBlockingNodeIdList());
    //        sendObpl = false;
    //    }
    //}
}
void Graph::Node::loseLock(){
    messageHandler->getAccountHandler()->removeFromMessageQueue(id);
    //Message releaseMsg(id, APPLICATION, "release", system_clock::to_time_t(system_clock::now()));
    //for(const auto& node : fileHandler->getNodeList()){
    //    sendMessageToNode(releaseMsg, node);
    //}
}

void Graph::Node::criticalSection(unsigned int randIndex){
    // 4. Wähle zufälligen Prozentsatz
    messageHandler->getAccountHandler()->setPercent(rand()%100);
    // 5. Sende den eigenen Kontostand und Prozentsatz
    ostringstream ss;
    ss << "send balance" << messageHandler->getAccountHandler()->getBalanceAmount() << ";" << messageHandler->getAccountHandler()->getPercent() ;
    //Message message(id,APPLICATION,ss.str());
    //sendMessageToNode(message, fileHandler->getNodeList().at(randIndex));
    //// 6. Request Balance
    //Message requestMessage(id,APPLICATION,"balance request");
    //sendMessageToNode(requestMessage, fileHandler->getNodeList().at(randIndex));
    waitAck = true;
    // 9. Warte auf ACK und gib waitAck frei
    while(waitAck){
        sleep(1);
    }
}