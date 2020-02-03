//
// Created by alhuber on 02.02.20.
//

#include "goldmanEdgeChasingHandler.hpp"

using namespace Handler;
using namespace std;

GoldmanEdgeChasingHandler::GoldmanEdgeChasingHandler() : node(nullptr){}

GoldmanEdgeChasingHandler::GoldmanEdgeChasingHandler(Graph::Node *node) : node(node){}

void GoldmanEdgeChasingHandler::handleIncommingMessages(Messages::OrderedBlockedMessage* message) {
    string content = message->getCommand();
    if (content.find("obpl") != string::npos) {

    }
}

void GoldmanEdgeChasingHandler::sendExtendOBPL() {

}

void GoldmanEdgeChasingHandler::initiateOBPL(vector<unsigned int> nodeIdList) {
    //Message obplMessage(node->getId(),CONTROL,"obpl");
    //for(auto id : nodeIdList){
    //    node->sendMessageToNode(obplMessage, node->getFileHandler()->getNodeFromFile(id));
    //}
}
