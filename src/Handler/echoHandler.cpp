//handleIncommingMessages
// Created by alhuber on 26.01.20.agrement
//

#include <unistd.h>
#include <thread>
#include "echoHandler.hpp"

using namespace std;
using namespace Graph;
using namespace Handler;

EchoHandler::EchoHandler(){}

EchoHandler::EchoHandler(const bool initiator, Node* thisNode, const unsigned int maxStartNumber)
    : virtualParentId(0), maxNodeId(0), resultCount(0), color(COLOR::WHITE), isWinner(false),
        isIinitiator(initiator), thisNode(thisNode), maxStartNumber(maxStartNumber) {
    if(initiator){
        color = COLOR::RED;
        maxNodeId = thisNode->getId();
        virtualParentId = thisNode->getId();
        thisNode->sendToNeighbors(Message(thisNode->getId(), APPLICATION, "explorer"));
    }
}

// message -> OriginalId ; senderId ; ... ; explorer|echo
void EchoHandler::handleIncommingMessages(Message* message){
    if(color != COLOR::GREEN){
        if(message->getOriginId() > maxNodeId){
            resultCount=1;
            maxNodeId = message->getOriginId();
            virtualParentId = message->getSenderId();
            if(resultCount == thisNode->getNeighbors().size() && color != COLOR::GREEN){
                color = COLOR::GREEN;
                sendEcho();
            }else{
                color = COLOR::RED;
                forwardExplorer(message);
            }
            spanningTreeList.clear();
        }else if(message->getOriginId() == maxNodeId) {
            resultCount++;
            if (message->getContent() == "echo") {
                for (auto i : message->getHopList()) {
                    spanningTreeList.push_back(i);
                }
            }
            if (color == COLOR::WHITE) {
                forwardExplorer(message);
                //virtualParentId = message->getSenderId();
            }
            if(resultCount == thisNode->getNeighbors().size() && color != COLOR::GREEN){
                color = COLOR::GREEN;
                if(thisNode->getId() == message->getOriginId()){
                    thisNode->setCoordinator(thisNode);
                    sleep(1);
                    thisNode->getFileHandler()->getNodeList();
                    //unused
                    //message msg(thisNode->getId(), MESSAGE_TYPE::CONTROL, "election won");
                    //thisNode->sendToSuperNode(msg);
                    vector<Node> allNodesList = thisNode->getFileHandler()->getNodeList();
                    for(unsigned int i=0; i < maxStartNumber; i++) {
                        unsigned int num = rand() % allNodesList.size();
                        thisNode->sendMessageToNode(Message(thisNode->getId(), MESSAGE_TYPE::APPLICATION, "vote"), allNodesList.at(num));
                        allNodesList.erase(allNodesList.begin()+num);
                    }
                    //thisNode->getDoubleCounting().start();
                }else{
                    sendEcho();
                }
            }
        }
    }
}

void EchoHandler::forwardExplorer(Message* message){
    thisNode->sendToNeighborsExceptSource(Message(thisNode->getId(), MESSAGE_TYPE::APPLICATION, "explorer", virtualParentId));
}

void EchoHandler::sendEcho(){
    unsigned int i=0;
    for(;i < thisNode->getNeighbors().size();i++){
        if(thisNode->getNeighbors().at(i).getId() == virtualParentId){
            break;
        }

    }
    spanningTreeList.emplace_back(thisNode->getId(), virtualParentId);
    Message msg(thisNode->getId(),MESSAGE_TYPE::APPLICATION, "echo", maxNodeId, getSpanningTreeList());
    thisNode->sendMessageToNode(msg, thisNode->getNeighbors().at(i));
}

void EchoHandler::sendStartDoubleCounting(){
    vector<Node> allNodesList = thisNode->getFileHandler()->getNodeList();
    for(unsigned int i=0; i < maxStartNumber; i++) {
        unsigned int num = rand() % allNodesList.size();
        thisNode->sendMessageToNode(Message(thisNode->getId(), MESSAGE_TYPE::APPLICATION, "vote"), allNodesList.at(num));
        allNodesList.erase(allNodesList.begin()+num);
    }
}
