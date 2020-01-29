#include <sstream>
#include "messageHandler.hpp"

using namespace std;
using namespace Uebung1;
using namespace chrono;

MessageHandler::MessageHandler()= default;

MessageHandler::MessageHandler(Node* node)
    : node(node){
    echoHandler = new EchoHandler(false, node, node->getConfig().getMaxSend());
}

/*
 * This function handles all incomming messages
 */
void MessageHandler::handleIncommingMessage(Message* msg){
    if(msg->getMessageType() == APPLICATION){
        if(msg->getContent() == "rumor"){
            unique_lock<mutex> lock(forwardMutex);
            if(node->getRecvRumors() == 0){
                node->incrementRecvRumors();
                // cout << "Done " << node->getId() << "And RECV " << node->getRecvRumors() << endl;
                for(unsigned int i=0;i< node->getNeighbors().size();i++){
                    if(node->getNeighbors().at(i).getId() != msg->getSenderId()){
                        node->sendMessageToNode(Message (node->getId(), MESSAGE_TYPE::APPLICATION,
                                "rumor") , node->getNeighbors().at(i));
                    }
                }
                node->sendMessageToNode(Message (node->getId(), MESSAGE_TYPE::CONTROL, "done"), *(node->getInitNode()));
                node->setHasSend(true);
            }
            node->incrementRecvRumors();
            lock.unlock();
        }else if(msg->getContent()=="echo" || msg->getContent() == "explorer"){
            unique_lock<mutex> lock(echoMutex);
            echoHandler->handleIncommingMessages(msg);
            lock.unlock();
        }else if(msg->getContent()=="vote"){
           node->getAgreement()->handleIncommingMessages(msg);
        }
    }else{
        ostringstream ss;
        if(msg->getContent() == "initiator") {
            ss << "Neighbor Id: " << node->getId();
            node->sendToNeighbors(Message(node->getId(), MESSAGE_TYPE::APPLICATION, ss.str()));
        }else if(msg->getContent() == "rumor") {
            // cout << "Done " << node->getId() << "And RECV " << node->getRecvRumors() <<  "And BOOl " << node->getHasSend() << endl;
            for(unsigned int i=0;i< node->getNeighbors().size();i++){
                node->sendMessageToNode(Message(node->getId(), MESSAGE_TYPE::APPLICATION,
                "rumor") , node->getNeighbors().at(i));
            }
            Message message2(node->getId(), MESSAGE_TYPE::CONTROL, "done");
            node->sendMessageToNode(message2, *(node->getInitNode()));
            node->setRecvRumors(node->getConfig().getMinTrust());
            node->sendToNeighbors(Message(node->getId(), MESSAGE_TYPE::APPLICATION, "rumor"));
        }if(msg->getContent() == "election") {
            echoHandler = new EchoHandler(true, node, node->getConfig().getMaxSend());
        }else if(msg->getContent() == "evaluate"){
            string believeS;
            node->getRecvRumors() >= node->getConfig().getMinTrust() ?  believeS = "true" : believeS = "false";
            node->setRecvRumors(0);
            node->setHasSend(false);
            node->sendMessageToNode(Message (node->getId(), MESSAGE_TYPE::CONTROL, believeS), *(node->getInitNode()));
        }else if(msg->getContent() == "end"){
            cout << "End Node " << node->getId() << endl;
            exit(EXIT_SUCCESS);
        }
    }
}


