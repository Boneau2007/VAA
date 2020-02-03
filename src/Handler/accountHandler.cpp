//
// Created by alhuber on 31.01.20.
//

#include <string>
#include <sstream>
#include "accountHandler.hpp"

using namespace Handler;
using namespace Messages;
using namespace Graph;
using namespace std;
using namespace chrono;

AccountHandler::AccountHandler() : node(nullptr), balance(0), percent(0) {}

AccountHandler::AccountHandler(Node* node) : node(node), percent(0){
    srand(time(nullptr));
    balance = rand()%100000;
}

void AccountHandler::handleIncommingMessages(Message* message){
    string content = message->getContent();
    //incomming excercise messages
    if(content.find("send balance") != string::npos) {
        string line;
        stringstream ss(content);
        getline(ss, line, ';'); //send balance

        getline(ss, line, ';'); //recvBalance
        unsigned int recvBalance = stoi(line);
        getline(ss, line, ';'); //recvPercent
        unsigned  int recvPercent = stoi(line);

        if(recvBalance >= balance){
            balance += recvBalance*recvPercent*0.01;
        }else{
            balance -= balance*recvPercent*0.01;
        }
        Message acknowledgementMsg(message->getSenderId(),APPLICATION,"release");
        node->sendMessageToNode(acknowledgementMsg,node->getFileHandler()->getNodeFromFile(message->getSenderId()));
    }else if(content.find("balance request") != string::npos){
        ostringstream ss;
        ss << "balance response;" << balance;
        Message responseMessage(node->getId(),APPLICATION, ss.str());
        node->sendMessageToNode(responseMessage, node->getFileHandler()->getNodeFromFile(message->getSenderId()));
    }else if(content.find("balance response") != string::npos){
        string line;
        stringstream ss(content);
        getline(ss, line, ';'); //balance response
        getline(ss, line, ';'); //recvBalance
        unsigned int recvBalance = stoi(line); //Get balance and percent
        if(recvBalance >= balance){
            balance += recvBalance*percent*0.01;
        }else{
            balance -= balance*percent*0.01;
        }
        node->setWaitAck(false);
    }
    //incomming Lamport Alg. messages
    if(content.find("request") != string::npos){
        ostringstream ss;
        ss << "reply;" << message->getTimestamp();
        requestQueue.insert(pair<Message, vector<unsigned int>>(*message, {}));
        Message replyMsg(message->getSenderId(),APPLICATION,ss.str(), system_clock::to_time_t(system_clock::now()));
        node->sendMessageToNode(replyMsg,node->getFileHandler()->getNodeFromFile(message->getSenderId()));
    }else if(content.find("reply") != string::npos){
        string line;
        stringstream ss(content);
        getline(ss, line, ';'); //reply
        getline(ss, line, ';'); //timestamp
        time_t recvCorrespondingTimestamp = stoi(line);
        auto iterator = requestQueue.begin();
        while(iterator != requestQueue.end()){
            if(recvCorrespondingTimestamp == iterator->first.getTimestamp()){
                iterator->second.emplace_back(message->getSenderId());
                break;
            }
        }
        //evaluate with the message.timestamps from the queue and save the id to the vector in queue
    }else if(content.find("release") != string::npos){
        removeFromMessageQueue(message->getSenderId());
    }
}

void AccountHandler::pushMessageToQueue(const Message& message){
    requestQueue.insert(pair<Message, vector<unsigned int>>(message, {}));
}

void AccountHandler::removeFromMessageQueue(unsigned int id){
    auto iterator = requestQueue.begin();
        while(iterator != requestQueue.end()){
        if(iterator->first.getSenderId() == id){
            requestQueue.erase(iterator);
            break;
        }
    }
}

bool AccountHandler::receivedReplyFromAll(){
    auto iterator = requestQueue.begin();
    while(iterator != requestQueue.end()){
        if( iterator->first.getSenderId() == node->getId() &&
            iterator->second.size() == node->getFileHandler()->getNodeList().size()){
            return true;
        }
    }
    return false;
}

bool AccountHandler::isLowestTimestamp(){
    time_t referedTime = 0;
    auto iterator = requestQueue.begin();
    //Get the time from the id dependend node
    while(iterator != requestQueue.end()){
        if(node->getId() == iterator->first.getSenderId()){
            referedTime = iterator->first.getTimestamp();
        }
    }
    iterator = requestQueue.begin();
    //Is some message time greater as this time? If true then is not lowest, therefore we send false
    while(iterator != requestQueue.end()){
        if(iterator->first.getTimestamp() > referedTime){
            return false;
        }
    }
    return true;
}

std::vector<unsigned int> AccountHandler::getBlockingNodeIdList() {
    vector<unsigned int> ids;
    time_t referedTime = 0;
    auto iterator = requestQueue.begin();
    //Get the time from the id dependend node
    while(iterator != requestQueue.end()){
        if(node->getId() == iterator->first.getSenderId()){
            referedTime = iterator->first.getTimestamp();
        }
    }
    iterator = requestQueue.begin();
    //Is some message time greater as this time? If true then is not lowest, therefore we send false
    while(iterator != requestQueue.end()){
        if(iterator->first.getTimestamp() > referedTime){
            ids.emplace_back(iterator->first.getOriginId());
        }
    }
    return ids;
}
