#include <sstream>
#include <algorithm>    // begin(), end(), find()
#include <chrono>   // time
#include <netdb.h>  // gethostbyname()
#include <cstring>  // memcpy()
#include <unistd.h>     // close() write()
#include <sys/socket.h> // inet_addr
#include <netinet/in.h> // inet_addr
#include <arpa/inet.h>  // inet_addr
#include "messageHandler.hpp"

using namespace std;
using namespace Uebung1;
using namespace chrono;

MessageHandler::MessageHandler(){}

MessageHandler::MessageHandler(Node* node)
    : node(node){}

MessageHandler::MessageHandler(Node* node, string msg)
    : node(node), message(Message(msg)){}


/*
 * This function handles all incomming messages
 */
void MessageHandler::handleIncommingMessage(){
    time_t time = system_clock::to_time_t(system_clock::now());
    string timeS(asctime(localtime(&time)));
    timeS.pop_back();
    ostringstream ss;
    ss << "[ID:"<<node->getId() << "] RECV: " << message.toString() << " [" << timeS <<"]"  << endl;
    //cout << ss.str();
    switch(message.getMessageType()){
        case APPLICATION:   if(message.getContent().compare("rumor") == 0){
                                forwardRumor(message.getSenderId());
                            }else if(message.getContent().compare(0,8,"explorer") == 0){
                                unsigned int recvVirtualId = atoi(message.getContent().substr(9, message.getContent().size()).c_str());
                                if(node->getNeighbors().size() == 1){
                                    sendEcho(message.getSenderId(), recvVirtualId);
                                }else{
                                    if(recvVirtualId > node->getVirtualParentId()){
                                        node->setVirtualParentId(recvVirtualId);
                                        sendExplorer(message.getSenderId());
                                    }else if(recvVirtualId == node->getVirtualParentId()){
                                        for(unsigned int i=0; i < node->getNeighbors().size(); i++){
                                            if(node->getNeighbors().at(i).getId() == message.getSenderId()){
                                                node->getNeighbors().at(i).setDone(true);
                                            }
                                        }
                                        if(wasLastNodeEdge()){
                                            sendEcho(node->getVirtualParentId(),node->getVirtualParentId());
                                            if(node->getInitiator()){
                                                node->setWinner(true);
                                                //TODO: ADD ALL Nodes to this node as neighbor
                                                startUnificationProcess();
                                            }
                                        }
                                    }
                                }
                            }else if(message.getContent().compare(0, 4, "echo") == 0){
                                unsigned int recvVirtualId = atoi(message.getContent().substr(5, message.getContent().size()).c_str());
                                if(node->getVirtualParentId() == recvVirtualId){
                                    for(unsigned int i=0; i < node->getNeighbors().size(); i++){
                                        if(node->getNeighbors().at(i).getId() == message.getSenderId()){
                                            node->getNeighbors().at(i).setDone(true);
                                            break;
                                        }
                                    }
                                    if(wasLastNodeEdge()){
                                        sendEcho(node->getVirtualParentId(),node->getVirtualParentId());
                                    }
                                }
                            }else if(message.getContent().compare("election") == 0){
                                stringstream ss;
                                ss << "vote " << node->getPreferedTime();
                                Message messageOut(node->getId(), MESSAGE_TYPE::APPLICATION, ss.str()); 
                                for(unsigned int i=0; i < node->getMaxPhilosopherNumber(); i++){
                                    unsigned int num = rand() % node->getNeighbors().size();
                                    sendMessage(messageOut , node->getNeighbors().at(num));
                                }
                            }else if(message.getContent().compare(0,4, "vote") == 0){
                                time_t recvTime = atoi(message.getContent().substr(5, message.getContent().size()).c_str());
                                stringstream ss;
                                ss << "vote " << node->getPreferedTime();
                                Message messageOut(node->getId(), MESSAGE_TYPE::APPLICATION, ss.str()); 
                                for(unsigned int i=0; i < node->getNeighbors().size(); i++){
                                    if(node->getNeighbors().at(i).getId() == message.getSenderId()){
                                        sendMessage(messageOut , node->getNeighbors().at(i));
                                        break;
                                    }
                                }
                                time_t diff = (recvTime + node->getPreferedTime()) >> 2;
                            }else{
                                std::cout << "["<< node->getId() <<"]:" << message.getContent() << " [" << timeS <<"]" << endl;
                            }
                            break;
        case CONTROL:   if(message.getContent().compare("initiator") == 0){
                            ostringstream ss;
                            ss << "Neighbor Id: " << node->getId();
                            node->sendOwnIdMessage(Message( node->getId(), MESSAGE_TYPE::APPLICATION, ss.str()));
                        }else if(message.getContent().compare("rumor") == 0){
                            initRumor();
                        }else if(message.getContent().compare("evaluate") == 0){
                            sendBelieve();
                        }else if(message.getContent().compare("election") == 0){
                            node->setVirtualParentId(node->getId());
                            node->setInitiator(true);
                            sendExplorer(message.getSenderId());
                        }else if(message.getContent().compare("end") == 0){
                            cout << "End Node " << node->getId() << endl;
                            exit(EXIT_SUCCESS);
                        }else{
                            std::cout << "Unknown Cotrol content: " << message.getContent() << endl;
                        }
                        break;
    }
}


/*
 * This function creates communication sockets and writes data to the host
 */
void MessageHandler::sendMessage(Message& message, Node& targetNode){
    int sock;
    struct sockaddr_in server;
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
        ss << "[ID:" << node->getId() << "] SEND: " << message.toString() << " [" << timeS << "]" << endl;
        //std::cout << ss.str();
    }
    catch(const exception& e){
        cerr << e.what() << endl;
    }
}

bool MessageHandler::wasLastNodeEdge(){
    for(unsigned int i=0;i< node->getNeighbors().size();i++){
        if(!node->getNeighbors().at(i).getDone()){
            return false;
        }
    }
    return true;
}

void MessageHandler::startUnificationProcess(){
    srand (time(NULL));
    Message message(node->getId(), MESSAGE_TYPE::APPLICATION, "election"); 
    for(unsigned int i=0; i < node->getMaxStartNumber(); i++){
        unsigned int num = rand() % node->getNeighbors().size();
        sendMessage(message , node->getNeighbors().at(num));
    }
}

/*
 * This function inits a rumor. It passes this rumor Message to its neighbnors and sends a signal to the init node, 
 * if it is done with the message forwarding.
 */
void MessageHandler::initRumor(){
    // cout << "Done " << node->getId() << "And RECV " << node->getRecvRumors() <<  "And BOOl " << node->getHasSend() << endl; 
    Node initNode(0,"127.0.0.1",node->getInitPort());
    Message message(node->getId(), MESSAGE_TYPE::APPLICATION, "rumor"); 
    for(unsigned int i=0;i< node->getNeighbors().size();i++){
        sendMessage(message , node->getNeighbors().at(i));
    }
    Message message2(node->getId(), MESSAGE_TYPE::CONTROL, "done");
    sendMessage(message2, initNode);
    node->setRecvRumors(node->getBelieverEpsilon());
}

/*
 * This function forwards the rumor Message to its neighbnors and sends a signal to the init node, 
 * if it is done with the message forwarding.
 * BUG: If not connected, then rumor does not reach node
 */
void MessageHandler::forwardRumor(const unsigned int senderId){
    unique_lock<mutex> lock(forwardMutex);
    Node initNode(0,"127.0.0.1",node->getInitPort());
    cout << "ID: " << node->getId() << "| RECV:" << node->getRecvRumors()<< " | BOOL " << node->getHasSend() << endl;
    if(!node->getHasSend()){
        // cout << "Done " << node->getId() << "And RECV " << node->getRecvRumors() << endl; 
        Message message(node->getId(), MESSAGE_TYPE::APPLICATION, "rumor"); 
        for(unsigned int i=0;i< node->getNeighbors().size();i++){
            if(node->getNeighbors().at(i).getId() != senderId){
                sendMessage(message , node->getNeighbors().at(i));
            }
        }
        Message message2(node->getId(), MESSAGE_TYPE::CONTROL, "done"); 
        sendMessage(message2, initNode);
        node->setHasSend(true);
    }
    node->incrementRecvRumors();
    lock.unlock();
}

/*
 * This function sends a true or false message to the Network initialisation Node.
 */
void MessageHandler::sendBelieve(){
    Node initNode(0,"127.0.0.1",node->getInitPort());
    string believeS;
    node->getRecvRumors() >= node->getBelieverEpsilon() ?  believeS = "true" : believeS = "false"; 
    node->setRecvRumors(0);
    node->setHasSend(false);
    Message message(node->getId(), MESSAGE_TYPE::CONTROL, believeS);
    sendMessage(message, initNode);
}

/*
 * This function sends a true or false message to the Network initialisation Node.
 */
void MessageHandler::sendExplorer(const unsigned int senderId){
    stringstream ss;
    ss << "explorer " << node->getVirtualParentId();
    Message message(node->getId(), MESSAGE_TYPE::APPLICATION, ss.str());
        for(unsigned int i=0;i< node->getNeighbors().size();i++){
            if(node->getNeighbors().at(i).getId() != senderId){
                sendMessage(message , node->getNeighbors().at(i));
            }
        }
}

/*
 * This function sends a true or false message to the Network initialisation Node.
 */
void MessageHandler::sendEcho(const unsigned int senderId, const unsigned int virtualId){
    stringstream ss;
    ss << "echo " << virtualId;
    Message message(node->getId(), MESSAGE_TYPE::APPLICATION, ss.str());
    for(unsigned int i=0;i< node->getNeighbors().size();i++){
            if(node->getNeighbors().at(i).getId() == senderId){
                sendMessage(message , node->getNeighbors().at(i));
            }
        }
}
