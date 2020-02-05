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
    cout << this->node->getId() << " Initiate rand balance: " << balance << endl;
}

void AccountHandler::handleIncommingMessage(const std::string& msg, const time_t time){
    if( msg.find("balance send") != string::npos || msg.find("balance response") != string::npos) {
        IMessage* sendResponseMsg = new AccountMessage(msg);
        cout << "Recv: [ " << sendResponseMsg->getCommand() << " ] at ID:" << node->getId() << " " << time << endl;
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

    }else if(msg.find("balance request") != string::npos){
        IMessage* requestMsg = new Message(msg);
        cout << "Recv: [ " << requestMsg->getCommand() << " ] at ID:" << node->getId() << " " << time << endl;
        if(requestMsg->getCommand() == "balance request"){
            IMessage*  balanceResponseMsg = new AccountMessage(node->getId(), APPLICATION, "balance response", balance, percent);
            node->sendMessageToNode(balanceResponseMsg, node->getFileHandler()->getNodeFromFile(requestMsg->getSenderId()));
        }
    }else if(msg.find("release") != string::npos){
        node->setWaitAck(false);
    }
}