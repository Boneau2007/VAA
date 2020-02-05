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
        cout << "ID: " << node->getId() << " recv [" << dynamic_cast<LockMessage*>(lockMsg)->toString() <<"] from " << lockMsg->getSenderId()<< endl;
        if(node->putMessageToLocalRequestQueue(*lockMsg)){
            //Sende die Nachricht an alle Nachbarn weiter
            IMessage* forwardMsg = new LockMessage(node->getId(), APPLICATION, "lock request", lockMsg->getLockId(), lockMsg->getOriginId(),
                    lockMsg->getLocalClock());
            node->sendToNeighborsExceptSource(forwardMsg);
            IMessage* lockAckMsg = new LockAckMessage(  node->getId(), APPLICATION, "lock ack", lockMsg->getLockId(),
                    node->getId(), node->getLocalClock().getTime(),
                    lockMsg->getOriginId(), lockMsg->getRecvLocalClock());
            node->sendMessageToNode(lockAckMsg, node->getFileHandler()->getNodeFromFile(lockMsg->getOriginId()));
        }
    }else if(msg.find("lock ack") != string::npos){
        IMessage* lockAckMsg = new LockAckMessage(msg);
        cout << "ID: " << node->getId() << " recv [" << dynamic_cast<LockAckMessage*>(lockAckMsg)->toString() <<"] from " << lockAckMsg->getSenderId()<< endl;
        node->putLockAckToLocalRequestQueue(*lockAckMsg);

    }else if (msg.find("lock release") != string::npos){
        IMessage* releasekMsg = new LockMessage(msg);
        cout << "ID: " << node->getId() << " recv [" << dynamic_cast<LockMessage*>(releasekMsg)->toString() <<"] from " << releasekMsg->getSenderId()<< endl;
        node->popMessageFromLocalRequestQueue(*releasekMsg);
    }
}

