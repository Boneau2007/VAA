#include <sstream>
#include "messageHandler.hpp"

using namespace std;
using namespace Uebung1;
using namespace chrono;

MessageHandler::MessageHandler()= default;

MessageHandler::MessageHandler(Node* node)
    : node(node){}

/*
 * This function handles all incomming messages
 */
void MessageHandler::handleIncommingMessage(const Message& msg){
    if(msg.getMessageType() == APPLICATION){
        if(message.getContent() == "rumor"){
            unique_lock<mutex> lock(forwardMutex);
            if(node->getRecvRumors() == 0){
                node->incrementRecvRumors();
                // cout << "Done " << node->getId() << "And RECV " << node->getRecvRumors() << endl;
                for(unsigned int i=0;i< node->getNeighbors().size();i++){
                    if(node->getNeighbors().at(i).getId() != msg.getSenderId()){
                        node->sendMessageToNode(Message (node->getId(), MESSAGE_TYPE::APPLICATION,
                                "rumor") , node->getNeighbors().at(i));
                    }
                }
                node->sendMessageToNode(Message (node->getId(), MESSAGE_TYPE::CONTROL, "done"),
                        Node(0, "127.0.0.1", node->getInitPort()));
                node->setHasSend(true);
            }
            node->incrementRecvRumors();
            lock.unlock();
        }else if(msg.getContent().compare(0, 4, "echo") == 0 || msg.getContent().compare(0, 8,
                "election") == 0 ){
            try{
                echoHandler.handleIncommingMessages(msg);
            }catch (exception& e){
                echoHandler = EchoHandler(false, node, node->getMaxStartNumber());
                echoHandler.handleIncommingMessages(msg);
            }
        }
    }else{
        ostringstream ss;
        if(msg.getContent() == "initiator") {
            ss << "Neighbor Id: " << node->getId();
            node->sendToNeighbors(Message(node->getId(), MESSAGE_TYPE::APPLICATION, ss.str()));
        }else if(msg.getContent() == "rumor") {
            // cout << "Done " << node->getId() << "And RECV " << node->getRecvRumors() <<  "And BOOl " << node->getHasSend() << endl;
            Node initNode(0,"127.0.0.1",node->getInitPort());
            for(unsigned int i=0;i< node->getNeighbors().size();i++){
                node->sendMessageToNode(Message(node->getId(), MESSAGE_TYPE::APPLICATION,
                "rumor") , node->getNeighbors().at(i));
            }
            Message message2(node->getId(), MESSAGE_TYPE::CONTROL, "done");
            node->sendMessageToNode(message2, initNode);
            node->setRecvRumors(node->getBelieverEpsilon());
            node->sendToNeighbors(Message(node->getId(), MESSAGE_TYPE::APPLICATION, "rumor"));
        }else if(msg.getContent() == "evaluate"){
            Node initNode(0,"127.0.0.1",node->getInitPort());
            string believeS;
            node->getRecvRumors() >= node->getBelieverEpsilon() ?  believeS = "true" : believeS = "false";
            node->setRecvRumors(0);
            node->setHasSend(false);
          ;
            node->sendMessageToNode(Message (node->getId(), MESSAGE_TYPE::CONTROL, believeS), initNode);
        }else if(msg.getContent() == "end"){
            cout << "End Node " << node->getId() << endl;
            exit(EXIT_SUCCESS);
        }
    }
}

