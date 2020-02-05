

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
using namespace Messages;
using namespace chrono;

Graph::Node::Node() :  id(0), ipAddress(""), port(0), recvRumors(0), hasSend(false), preferedTime(0), fileHandler(nullptr), messageHandler(nullptr),
                coordinator(nullptr), initNode(nullptr), waitAck(false), hasDeadlock(false){}

Graph::Node::Node(const unsigned int id, string  ipAddress, const unsigned int port) : Node() {
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
    for(auto& i : fileHandler->getNodeList()){
        localRequestQueueList.insert(pair<unsigned int, map<Messages::IMessage, std::vector<unsigned int>>>(i.getId(),{}));
        if(i.getId() == id){
            ipAddress = i.getIpAddress();
            port = i.getPort();
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

    //Send Id to neighbors
    //for(Graph::Node& neighbor : neighbors){
    //    threadPool.emplace_back(&Node::executeSendMessageThread, this, neighbor);
    //}
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

void Graph::Node::executeSendMessageThread(const Node& node){
    ostringstream ss;
    ss << "Node Id: " << id;
    IMessage* message = new Message(id, MESSAGE_TYPE::APPLICATION, ss.str());
    sleep(2);
    sendMessageToNode(message, node);
}

/*
 * This function creates communication sockets and writes data to the host
 */
void Graph::Node::sendMessageToNode(IMessage* message, const Node& targetNode){
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
        localClock.sendEvent();
        if(connect(sock, (struct sockaddr*)&server, sizeof(server)) == -1){
            throw runtime_error("ERROR: Unable to connect");
        }
        if( command.find("Node Id: ") != string::npos ||
            command.find("end") != string::npos ||
            command.find("election") != string::npos ||
            command.find("initiator") != string::npos||
            command.find("rumor") != string::npos ||
            command.find("release") != string::npos){
            auto* msg = dynamic_cast<Message*>(message);
            cout << "ID: " << id << " sende [" << msg->toString() <<"] to " << targetNode.getId()<< endl;
            if(write(sock, msg->toString().c_str(), msg->toString().size()) == -1){
                throw runtime_error("ERROR: Unable to write");
            }

        }else if(command.find("lock request") != string::npos ||
                 command.find("lock release") != string::npos){
            auto* msg = dynamic_cast<LockMessage*>(message);
            cout << "ID: " << id << " sende [" << msg->toString() <<"] to " << targetNode.getId()<< endl;
            if(write(sock, msg->toString().c_str(), msg->toString().size()) == -1){
                throw runtime_error("ERROR: Unable to write lock- request or relase Message");
            }
        }else if(command.find("obpl") != string::npos){
            auto* msg = dynamic_cast<OrderedBlockedMessage*>(message);
            if(write(sock, msg->toString().c_str(), msg->toString().size()) == -1){
                throw runtime_error("ERROR: Unable to write obpl Message");
            }
        }else if(command.find("lock ack") != string::npos){
            auto* msg = dynamic_cast<LockAckMessage*>(message);
            cout << "ID: " << id << " sende [" << msg->toString() <<"] to " << targetNode.getId()<< endl;
            if(write(sock, msg->toString().c_str(), msg->toString().size()) == -1){
                throw runtime_error("ERROR: Unable to write lock ack Message");
            }
        }else if(command.find("balance send") != string::npos ||
                 command.find("balance response") != string::npos){
            auto* msg = dynamic_cast<AccountMessage*>(message);
            cout << "ID: " << id << " sende [" << msg->toString() <<"] to " << targetNode.getId()<< endl;
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

void Graph::Node::sendToNeighbors(IMessage* msg){
    for(const auto & neighbor : neighbors){
        sendMessageToNode(msg, neighbor);
    }
}

void Graph::Node::sendToNeighborsExceptSource(IMessage* msg){
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
    unsigned int randIndex = 0;
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
        acquireLock(system_clock::to_time_t(system_clock::now())+id, randIndex);
        criticalSection(randIndex);
        // 10. Gebe lock frei
        loseLock(randIndex);
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
            timeList.push_back(stoi(line));
        }
    }
    unsigned int num = rand()%timeList.size();
    preferedTime = timeList.at(num);
}

void Graph::Node::acquireLock(unsigned int waitTillTime, unsigned int randIndex) {
    lockId = fileHandler->getNodeList().at(randIndex).getId();
    unsigned int originId = id;
    unsigned int senderId = id;
    unsigned int localClockTime = localClock.getTime();
    bool sendObpl = false; //TODO: Setze auf false
    IMessage* lockMsg = new LockMessage(senderId, APPLICATION, "lock request", lockId, originId, localClockTime);
    sendToNeighbors(lockMsg);
    //Speichere die Requestnachricht in die richtige lokale Request queue ein, auf welche zugegriffen werden soll
    for(auto& i : localRequestQueueList){
        if(i.first == lockId){
            cout << "Put <" << lockMsg->getLockId() << "," << lockMsg->getLocalClock()<<"> to Q" << i.first <<  " at "<< id << endl;
            i.second.insert(pair<IMessage, vector<unsigned int>>(*lockMsg,{}));
        }
    }
    while(!(isLowestTimestamp(randIndex) && receivedReplyFromAll(randIndex))){
        if(system_clock::to_time_t(system_clock::now()) >= waitTillTime && !sendObpl){
            cout << "Wait max is reached" << endl;
            IMessage* obplMsg = new OrderedBlockedMessage(id, CONTROL, "obpl");
            dynamic_cast<OrderedBlockedMessage*>(obplMsg)->addObpId(id);
            sendMessageToNode(obplMsg, fileHandler->getNodeFromFile(lockId));
            sendObpl = true;
        }
        if(hasDeadlock){
            //TODO: break and handle resources
        }
    }
}
void Graph::Node::loseLock(unsigned int randIndex){
    unsigned int lockId = fileHandler->getNodeList().at(randIndex).getId();
    unsigned int originId = id;
    unsigned int senderId = id;
    unsigned int localClockTime = 0;
    for(auto& queueId : localRequestQueueList){
        if(queueId.first == lockId){
            for(auto& i : queueId.second){
                if(i.first.getOriginId() == originId){
                    localClockTime = i.first.getLocalClock();
                }
            }
        }
    }
    IMessage* releaseMsg = new LockMessage(id, APPLICATION, "release",  lockId, originId, localClockTime);
    sendToNeighbors(releaseMsg);
}

void Graph::Node::criticalSection(unsigned int randIndex){
    waitAck = true;
    messageHandler->getAccountHandler()->setPercent(rand()%100);
    cout << "Choose rand percentage is " << messageHandler->getAccountHandler()->getPercent() << endl;

    IMessage* sendMsg = new AccountMessage(id, APPLICATION, "balance request", messageHandler->getAccountHandler()->getBalanceAmount(),messageHandler->getAccountHandler()->getPercent());
    cout << "ID:" << id << " send " << dynamic_cast<AccountMessage*>(sendMsg)->toString() << endl;
    sendMessageToNode(sendMsg, fileHandler->getNodeList().at(randIndex));

    IMessage* requestMsg = new Message(id, APPLICATION, "balance request");
    cout << "ID:" << id << " send " << dynamic_cast<Message*>(requestMsg)->toString() << endl;
    sendMessageToNode(requestMsg, fileHandler->getNodeList().at(randIndex));

    while(waitAck){
        sleep(1);
    }
}

bool Graph::Node::putMessageToLocalRequestQueue(const IMessage& message){
    for(auto& queueId : localRequestQueueList){
        if(queueId.first == message.getLockId()){
            auto it = queueId.second.begin();
            for(; it!= queueId.second.end();it++){
                if(it->first.getOriginId() == message.getOriginId()){
                    return false;
                }
            }
        }
    }
    for(auto& queueId : localRequestQueueList){
        if(queueId.first == message.getLockId()){
            cout << "Put <" << message.getOriginId() << "," <<  message.getLocalClock()<<"> to Q" << queueId.first <<  " at "<< id << endl;
            queueId.second.insert(pair<IMessage, vector<unsigned int>>(message, {}));
        }
    }
    return true;
}

void Graph::Node::putLockAckToLocalRequestQueue(const IMessage& message){
    //Jede queue in der in der localRequestQueueList zeigt an ob für die jeweilige Ressource(Hier eine Node[nodeId])einen Request hat
    for(auto& queueId : localRequestQueueList){
        //Finde die zur lockId zugehörige Request Queue Qn,k
        if(queueId.first == message.getLockId()){
            //Finde das eigene Lock in der spezifischen queue
            for(auto& i : queueId.second){
                //Wenn du dein eigenes Lock mit deiner nodeId gefunden hast, füge die nodeId der Node ein, von der die acknoledge nachricht kam
                if(i.first.getOriginId() == message.getOriginId()){
                    i.second.emplace_back(message.getSenderId());
                }
            }
        }
    }
}


void Graph::Node::popMessageFromLocalRequestQueue(const Messages::IMessage &message){
    //Jede queue in der in der localRequestQueueList zeigt an ob für die jeweilige Ressource(Hier eine Node[nodeId])einen Request hat
    for(auto& queueId : localRequestQueueList){
        //Finde die zur lockId zugehörige Request Queue Qn,k
        if(queueId.first == message.getLockId()){
            auto it = queueId.second.begin();
            for(; it!= queueId.second.end();it++){
                if(it->first.getOriginId() == message.getOriginId()){
                    cout << "Pop Message from local queue" << endl;
                    queueId.second.erase(it);
                }
            }
        }
    }
}

bool Graph::Node::receivedReplyFromAll(unsigned int randIndex){
    for(auto& queueId : localRequestQueueList){
        //Finde die zur lockId zugehörige Request Queue Qn,k
        if(queueId.first == fileHandler->getNodeList().at(randIndex).getId()){
            //Finde das eigene Lock in der spezifischen queue
            for(auto& i : queueId.second){
                if(i.first.getOriginId() == this->id){
                    //Wurde von jedem ein ack gesendet?
                    return i.second.size() == fileHandler->getNodeList().size() - 1;
                }
            }
        }
    }
    return false;
}

bool Graph::Node::isLowestTimestamp(unsigned int randIndex){
    for(auto& queueId : localRequestQueueList){
        //Finde die zur lockId zugehörige Request Queue Qn,k
        if(queueId.first == fileHandler->getNodeList().at(randIndex).getId()){
            //Finde das eigene Lock in der spezifischen queue
            unsigned int ownTime = 0;
            for(auto& i : queueId.second){
                if(i.first.getOriginId() == this->id){
                    ownTime = i.first.getLocalClock();
                    break;
                }
            }
            for(auto& i : queueId.second){
                if(i.first.getLocalClock() > ownTime){
                    return false;
                }
            }
        }
    }
    return true;
}

