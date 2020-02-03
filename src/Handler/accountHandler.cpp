//
// Created by alhuber on 31.01.20.
//

#include <string>
#include <sstream>
#include "Include/accountHandler.hpp"

using namespace Handler;
using namespace Messages;
using namespace Graph;
using namespace std;
using namespace Messages;
using namespace chrono;

AccountHandler::AccountHandler() : node(nullptr), balance(0), percent(0) {}

AccountHandler::AccountHandler(Graph::Node* node) : node(node), percent(0){
    srand(time(nullptr));
    balance = rand()%100000;
}

void AccountHandler::handleIncommingMessage(const std::string& msg, const time_t time){
    if( msg.find("balance send") != string::npos || msg.find("balance response") != string::npos) {
        IMessage* sendResponseMsg = new AccountMessage(msg);
        if(sendResponseMsg->getCommand() == "balance send"){
            unsigned int recvBalance = sendResponseMsg->getBalance();
            unsigned int recvPercent = sendResponseMsg->getPercent();
            if(recvBalance >= balance){
                balance += recvBalance*recvPercent*0.01;
            }else{
                balance -= balance*recvPercent*0.01;
            }
            IMessage* releaseMsg = new Message(sendResponseMsg->getSenderId(), APPLICATION, "release");
            node->sendMessageToNode(releaseMsg,node->getFileHandler()->getNodeFromFile(sendResponseMsg->getSenderId()));
        }else if(sendResponseMsg->getCommand() == "balance response"){
            unsigned int recvBalance = sendResponseMsg->getBalance();
            unsigned int recvPercent = sendResponseMsg->getPercent();
            if(recvBalance >= balance){
                balance += recvBalance*recvPercent*0.01;
            }else{
                balance -= balance*recvPercent*0.01;
            }
        }

    }else if(msg.find("balance request") != string::npos || msg.find("release") != string::npos ){
        IMessage* requestReleaseMessage = new Message(msg);
        if(requestReleaseMessage->getCommand() == "balance request"){
            IMessage*  balanceResponseMsg = new AccountMessage(node->getId(), APPLICATION, "balance response", balance, percent);
        }else if(requestReleaseMessage->getCommand() == "release"){

        }
        ostringstream ss;
        ss << "balance response;" << balance;
        //Message responseMessage(node->getId(),APPLICATION, ss.str());
        //node->sendMessageToNode(responseMessage, node->getFileHandler()->getNodeFromFile(message->getSenderId()));
    }
    //incomming Lamport Alg. messages
    if(content.find("request") != string::npos){
        ostringstream ss;
        ss << "reply;" << message->getTimestamp();
        //requestQueue.insert(pair<Message, vector<unsigned int>>(*message, {}));
        //Message replyMsg(message->getSenderId(),APPLICATION,ss.str(), system_clock::to_time_t(system_clock::now()));
        //node->sendMessageToNode(replyMsg,node->getFileHandler()->getNodeFromFile(message->getSenderId()));
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

void AccountHandler::pushMessageToQueue(const Messages::AccountMessage& message){
    requestQueue.insert(pair<Messages::AccountMessage, vector<unsigned int>>(message, {}));
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
            ids.emplace_back(iterator->first.getSenderId());
        }
    }
    return ids;
}
