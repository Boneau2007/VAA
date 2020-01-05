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

MessageHandler::MessageHandler()
    :recvRumors(0), sendedRumor(false){}

MessageHandler::MessageHandler(Node& node)
    : node(node), recvRumors(0), sendedRumor(false){}

MessageHandler::MessageHandler(Node& node, string msg)
    : node(node), message(Message(msg)), recvRumors(0), sendedRumor(false){
    }

void MessageHandler::handleIncommingMessage(){
    time_t time = system_clock::to_time_t(system_clock::now());
    string timeS(asctime(localtime(&time)));
    timeS.pop_back();
    ostringstream ss;
    ss << "[ID:"<<node.getId()<<"] RECV: "<< message.toString() << " [" << timeS <<"]"  << endl;
    cout << ss.str();
    switch(message.getMessageType()){
        case APPLICATION:   if(message.getContent().compare("rumor") == 0){
                                forwardRumor();
                            }else{
                                std::cout << "["<< node.getId() <<"]:" << message.getContent() << " [" << timeS <<"]" << endl;
                            }
                            break;
        case CONTROL:   if(message.getContent().compare("initiator") == 0){
                            ostringstream ss;
                            ss << "Has Id: " << node.getId();
                            node.sendOwnIdMessage(Message( node.getId(), MESSAGE_TYPE::APPLICATION, ss.str()));
                        }else if(message.getContent().compare("rumor") == 0){
                            initRumor();
                        }else if(message.getContent().compare("ping") == 0){
                            if(find(activeNodeList.begin(), activeNodeList.end(), message.getSenderId()) == activeNodeList.end()){
                                activeNodeList.push_back(message.getSenderId());
                            }
                        }else if(message.getContent().compare("evaluate") == 0){
                            sendBelieve();
                        }else if(message.getContent().compare("end") == 0){
                            cout << "End Node " << node.getId() << endl;
                            exit(EXIT_SUCCESS);
                        }else{
                            std::cout << "Unknown Cotrol content: " << message.getContent() << endl;
                        }
                        break;
    }
}

void MessageHandler::sendMessage(Message& message, Node& targetNode){
    // struct timeval timeout;      
    // timeout.tv_sec = 5;
    // timeout.tv_usec = 0;
    int sock;
    struct sockaddr_in server;
    try{
        if((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1){
            std::cout << "Unable to create socket" << endl;
            throw exception();
        }
        server.sin_family = AF_INET;
        server.sin_port = htons(targetNode.getPort());  
        if(inet_pton(AF_INET, targetNode.getIpAddress().c_str(),&server.sin_addr) == -1){
            std::cout << "Invalid Addess" << endl;
            throw exception();
        }
        // setsockopt (sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
        if(connect(sock, (struct sockaddr*)&server, sizeof(server)) == -1){
            std::cout << "Unable to connect" << endl;
            throw exception();
        }
        time_t time = system_clock::to_time_t(system_clock::now());
	    if(write(sock, message.toString().c_str(), message.toString().size()) == -1){
            std::cout << "Unable to write" << endl;
            throw exception();
        }
	    close(sock);
        string timeS(asctime(localtime(&time)));
        timeS.pop_back();
        ostringstream ss;
        ss << "[ID:" << node.getId() << "] SEND: " << message.toString() << " [" << timeS << "]" << endl;
        //std::cout << ss.str();
    }
    catch(const exception& e){
        cerr << e.what() << endl;
    }
}

/*
 * This function inits a rumor. It passes this rumor Message to its neighbnors and sends a signal to the init node, 
 * if it is done with the message forwarding.
 */
void MessageHandler::initRumor(){
    Message message(node.getId(), MESSAGE_TYPE::APPLICATION, "rumor"); 
    for(unsigned int i=0;i< node.getNeighbors().size();i++){
        sendMessage(message , node.getNeighbors().at(i));
    }
    sendedRumor = true;
    Message message2(node.getId(), MESSAGE_TYPE::CONTROL, "done"); 
    sendMessage(message2, *node.getInitNode());
}

/*
 * This function forwards the rumor Message to its neighbnors and sends a signal to the init node, 
 * if it is done with the message forwarding.
 */
void MessageHandler::forwardRumor(){
    if(!sendedRumor){
        Message message(node.getId(), MESSAGE_TYPE::APPLICATION, "rumor"); 
        for(unsigned int i=0;i< node.getNeighbors().size();i++){
            if(node.getNeighbors().at(i).getId() != node.getId()){
                sendMessage(message , node.getNeighbors().at(i));
            }
        }
        sendedRumor = true;
        Message message2(node.getId(), MESSAGE_TYPE::CONTROL, "done"); 
        sendMessage(message2, *node.getInitNode());
    }else{
        recvRumors++;
    }
    
}

/*
 * This function sends a true or false message to the Network initialisation Node.
 */
void MessageHandler::sendBelieve(){
    string believeS;
    recvRumors >= node.getBelieverEpsilon() ?  believeS = "true" : believeS = "false"; 
    Message message(node.getId(), MESSAGE_TYPE::CONTROL, believeS);
    sendMessage(message, *node.getInitNode());
}

void MessageHandler::sendPing(){
    Message message(node.getId(), MESSAGE_TYPE::CONTROL, "ping");
    sendMessage(message, *node.getInitNode());
}



