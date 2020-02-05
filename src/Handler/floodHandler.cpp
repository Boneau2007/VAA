//
// Created by alhuber on 04.02.20.
//
#include "floodHandler.hpp"

using namespace std;
using namespace Messages;

Handler::LockHandler::LockHandler() = default;

Handler::LockHandler::LockHandler(Graph::Node *node) : node(node) {}

void Handler::LockHandler::handleIncommingMessage(const std::string &msg, const time_t time) {
    if(msg.find("lock request") != string::npos){
        IMessage* lockMsg = new LockMessage(msg);
        cout << "Recv: [ " << lockMsg->getCommand() << " ] at ID:" << node->getId() << " " << time << endl;
        if(node->putMessageToLocalRequestQueue(*lockMsg)){
            //Sende die Nachricht an alle Nachbarn weiter
            IMessage* forwardMsg = new LockMessage(node->getId(), APPLICATION, "lock request", lockMsg->getLockId(), lockMsg->getOriginId(),
                    lockMsg->getRecvLocalClock());
            node->sendToNeighborsExceptSource(forwardMsg);
            IMessage* lockAckMsg = new LockAckMessage(  node->getId(), APPLICATION, "lock ack", lockMsg->getLockId(),
                    node->getId(), node->getLocalClock().getTime(),
                    lockMsg->getOriginId(), lockMsg->getRecvLocalClock());
            node->sendMessageToNode(lockAckMsg, node->getFileHandler()->getNodeFromFile(lockMsg->getOriginId()));
        }
    }else if(msg.find("lock ack") != string::npos){
        IMessage* lockAckMsg = new LockAckMessage(msg);
        cout << "Recv: [ " << lockAckMsg->getCommand() << " ] at ID:" << node->getId() << " " << time << endl;
        node->putLockAckToLocalRequestQueue(*lockAckMsg);

    } else if (msg.find("lock release") != string::npos){
        IMessage* releasekMsg = new LockMessage(msg);
        cout << "Recv: [ " << releasekMsg->getCommand() << " ] at ID:" << node->getId() << " " << time << endl;
        node->popMessageFromLocalRequestQueue(*releasekMsg);
    }
}

