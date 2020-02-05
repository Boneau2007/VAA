//
// Created by alhuber on 02.02.20.
//

#include "goldmanEdgeChasingHandler.hpp"

using namespace Handler;
using namespace Messages;
using namespace std;

GoldmanEdgeChasingHandler::GoldmanEdgeChasingHandler() : node(nullptr){}

GoldmanEdgeChasingHandler::GoldmanEdgeChasingHandler(Graph::Node *node) : node(node){}

void GoldmanEdgeChasingHandler::handleIncommingMessages(const std::string &obplMsgAsString, time_t time) {
    IMessage* obplMsg = new OrderedBlockedMessage(obplMsgAsString);
    for(unsigned int obpl : obplMsg->getOrderedBlockedProcessList()){
        
    }
}

