#include <sstream>
#include <regex>
#include <chrono>   // time
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
    cout << ss.str();
    switch(message.getMessageType()){
        case APPLICATION:   if(message.getContent() == "rumor"){
                                forwardRumor(message.getSenderId());
                            }else if(message.getContent().compare(0,8,"explorer") == 0){
                                unsigned int recvVirtualId = atoi(message.getContent().substr(9, message.getContent().size()).c_str());
                                // Grad == 1, hat keine Nachbarn
                                if(node->getNeighbors().size() == 1){
                                    sendEcho(message.getSenderId(), recvVirtualId);
                                // Grad > 1, hat Nachbarn
                                }else{
                                    // j > i Knoten wird von starkerem Initiator erobert
                                    if(recvVirtualId > node->getVirtualParentId()){
                                        node->setVirtualParentId(recvVirtualId);
                                        sendExplorer(message.getSenderId());
                                    // j == i, d.h. zwei Explorer identischer Markierung begegnen sich
                                    }else if(recvVirtualId == node->getVirtualParentId()){
                                        for(unsigned int i=0; i < node->getNeighbors().size(); i++){
                                            if(node->getNeighbors().at(i).getId() == message.getSenderId()){
                                                node->getNeighbors().at(i).setDone(true);
                                                break;
                                            }
                                        }
                                        if(wasLastNotVirtualNodeEdge()){
                                            sendEcho(node->getId(), node->getVirtualParentId());
                                        }
                                    }
                                }
                            }else if(message.getContent().compare(0, 4, "echo") == 0){
                                stringstream ss; 
                                ss << message.getContent();
                                char delemiter(';');
                                string command;
                                getline(ss, command , delemiter);
                                unsigned int recvVirtualId = atoi(command.substr(5, command.size()).c_str());
                                if(node->getVirtualParentId() == recvVirtualId){
                                    string contentSubString = message.getContent().substr(command.size(), message.getContent().size());
                                    node->addEchoNeighborMsg(contentSubString);
                                    for(unsigned int i=0; i < node->getNeighbors().size(); i++){
                                        if(node->getNeighbors().at(i).getId() == message.getSenderId()){
                                            node->getNeighbors().at(i).setDone(true);
                                            break;
                                        }
                                    }
                                    if(wasLastNotVirtualNodeEdge() && !node->getInitiator()){
                                        sendEcho(node->getId(),node->getVirtualParentId());
                                    }else if(wasLastNotVirtualNodeEdge() && node->getInitiator()){
                                        node->setWinner(true);
                                        startUnificationProcess();
                                    }
                                }
                            }else if(message.getContent() == "election"){
                                stringstream ss;
                                ss << "vote " << node->getPreferedTime();
                                Message messageOut(node->getId(), MESSAGE_TYPE::APPLICATION, ss.str());
                                // Wählt p random nachbarn
                                vector<Node> randNeighbors = getRandNodeIdList(node->getMaxPhilosopherNumber());
                                for(auto & randNeighbor : randNeighbors){
                                    sendMessage(messageOut , randNeighbor);
                                }
                            }else if(message.getContent().compare(0,4, "vote") == 0){
                                if(node->getMaxPhilosopherNumber() > node->getVoteCount()){
                                    time_t recvTime = atoi(message.getContent().substr(5, message.getContent().size()).c_str());
                                    stringstream ss;
                                    ss << "back " << node->getPreferedTime();
                                    Message messageOut(node->getId(), MESSAGE_TYPE::APPLICATION, ss.str()); 
                                    for(unsigned int i=0; i < node->getNeighbors().size(); i++){
                                        if(node->getNeighbors().at(i).getId() == message.getSenderId()){
                                            sendMessage(messageOut , node->getNeighbors().at(i));
                                            break;
                                        }
                                    }
                                    //Mittelwertberechnung
                                    time_t diff = (recvTime + node->getPreferedTime()) >> 2;
                                    // Aufrunden des Mittelwerts wenn dieser ungerade war
                                    if((recvTime + node->getPreferedTime()) % 2 != 0){
                                        diff++;
                                    }
                                    node->setPreferedTime(diff);
                                    node->incrementVoteCount();
                                }
                               

                            }else if(message.getContent().compare(0,4, "back") == 0){
                                time_t recvTime = atoi(message.getContent().substr(5, message.getContent().size()).c_str());
                                //Mittelwertberechnung
                                time_t diff = (recvTime + node->getPreferedTime()) >> 2;
                                // Aufrunden des Mittelwerts wenn dieser ungerade war
                                if((recvTime + node->getPreferedTime()) % 2 != 0){
                                    diff++;
                                }
                                node->setPreferedTime(diff);
                                // Pb führt die Abstimmung auf gleiche Weise fort
                                stringstream ss;
                                ss << "vote " << node->getPreferedTime();
                                Message messageOut(node->getId(), MESSAGE_TYPE::APPLICATION, ss.str());
                                // Wählt p random nachbarn
                                vector<Node> randNeighbors = getRandNodeIdList(node->getMaxPhilosopherNumber());
                                for(auto & randNeighbor : randNeighbors){
                                    sendMessage(messageOut , randNeighbor);
                                }

                            }else if(message.getContent().compare(0,7,"inquiry") == 0){
                                stringstream ss; 
                                ss << message.getContent();
                                char delemiter(';');
                                string nodeAsString;
                                smatch matches;
                                getline(ss, nodeAsString , delemiter);
                                nodeAsString = message.getContent().substr(nodeAsString.size(), message.getContent().size());
                                const regex isNodeString("([0-9]+) ([0-9]+.[0-9]+.[0-9]+.[0-9]+) ([0-9]+)");
                                regex_match(nodeAsString, matches, isNodeString);
                                Node observerNode(atoi(matches.str(1).c_str()), matches.str(2), atoi(matches.str(3).c_str()));
            
                                stringstream sOut; 
                                sOut << "result " << (node->getMaxPhilosopherNumber() > node->getVoteCount()) ;
                                Message messageOut(node->getId(), MESSAGE_TYPE::APPLICATION, sOut.str());
                                sendMessage(messageOut , observerNode);
                            }else if(message.getContent().compare(0,6,"result") == 0){
                                //TODO: Set echoNeighbor variable so it can be used in other function
                                node->incrementResultCount();
                            }else{
                                std::cout << "["<< node->getId() <<"]:" << message.getContent() << " [" << timeS <<"]" << endl;
                            }
                            break;
        case CONTROL:   if(message.getContent() == "initiator"){
                            ostringstream ss;
                            ss << "Neighbor Id: " << node->getId();
                            node->sendOwnIdMessage(Message( node->getId(), MESSAGE_TYPE::APPLICATION, ss.str()));
                        }else if(message.getContent() == "rumor"){
                            initRumor();
                        }else if(message.getContent() == "evaluate"){
                            sendBelieve();
                        }else if(message.getContent() == "election"){
                            node->setVirtualParentId(node->getId());
                            node->setInitiator(true);
                            sendExplorer(message.getSenderId());
                        }else if(message.getContent() == "end"){
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

bool MessageHandler::wasLastNotVirtualNodeEdge(){
    for(unsigned int i=0;i< node->getNeighbors().size();i++){
        if(node->getNeighbors().at(i).getId() == node->getVirtualParentId()){
            continue;
        }
        if(!node->getNeighbors().at(i).getDone()){
            return false;
        }
    }
    return true;
}

void MessageHandler::startUnificationProcess(){
    vector<Node> randList = getRandNodeIdList(node->getMaxStartNumber());
    Message message(node->getId(), MESSAGE_TYPE::APPLICATION, "election"); 
    for(unsigned int i=0; i < randList.size(); i++){
        sendMessage(message , node->getEchoNeighbors().at(i));
    }
    string content = "inquiry;" + node->toString() + ";";
    message.setContent(content);
    bool voteEnd = false;
    while(!voteEnd){
        sleep(1);
        for(unsigned int i = 0;i < node->getEchoNeighbors().size(); i++){
            sendMessage(message, node->getEchoNeighbors().at(i));
        }
        while(node->getResultCount() < node->getEchoNeighbors().size());

        sleep(1);
        for(unsigned int i = 0;i < node->getEchoNeighbors().size(); i++){
            sendMessage(message, node->getEchoNeighbors().at(i));
        }
        while(node->getResultCount() < node->getEchoNeighbors().size());
        
        //compare
        //collect
        //evaluate
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
        for(unsigned int i=0;i< node->getNeighbors().size(); i++){
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
    ss << "echo " << virtualId << ";" << node->toString() << ";";
    for(unsigned int i=0;i < node->getEchoNeighbors().size(); i++){
        ss << node->getEchoNeighbors().at(i).toString() << ";";
    }
    Message message(node->getId(), MESSAGE_TYPE::APPLICATION, ss.str());
    for(unsigned int i=0;i< node->getNeighbors().size();i++){
            if(node->getNeighbors().at(i).getId() == senderId){
                sendMessage(message , node->getNeighbors().at(i));
            }
        }
}

bool MessageHandler::hasElement(vector<Node> neighbors, const unsigned int id){
    for(unsigned int i=0;i < neighbors.size();i++){
        if(neighbors.at(i).getId() == id){
            return true;
        }
    }
    return false;
}


std::vector<Node> MessageHandler::getRandNodeIdList(const unsigned int maxNumber){
    vector<Node> randNodeIdList;
    srand (time(nullptr));
    while (randNodeIdList.size() < maxNumber){
        unsigned int num = rand()%node->getEchoNeighbors().size();
        for(unsigned int i=0; i < randNodeIdList.size(); i++){
            if( node->getEchoNeighbors().at(num).getId() != randNodeIdList.at(i).getId() &&
                !hasElement(randNodeIdList, node->getEchoNeighbors().at(num).getId())){
                randNodeIdList.push_back(node->getEchoNeighbors().at(num));
            }
        }
    }
    return randNodeIdList;
}
