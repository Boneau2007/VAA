#include <sstream>
#include "Include/messageHandler.hpp"

using namespace std;
using namespace Handler;
using namespace chrono;

MessageHandler::MessageHandler()= default;

MessageHandler::MessageHandler(Graph::Node* node)
    : node(node){
    echoHandler = new EchoHandler(false, node, node->getConfig().getMaxSend());
    accountHandler = new AccountHandler(node);
    goldmanEdgeChasingHandler = new Handler::GoldmanEdgeChasingHandler(node);
}

/*
 * This function handles all incomming messages
 */
void MessageHandler::handleIncommingMessage(const string& msg, const time_t time){
    if(msg.find("ctrl") != string::npos){
        if(msg.find("end") != string::npos ||
           msg.find("election") != string::npos ||
           msg.find("initiator") != string::npos||
           msg.find("init rumor") != string::npos) {
            Messages::Message message(msg);
            if (msg.find("rumor") != string::npos) {
                unique_lock<mutex> lock(forwardMutex);
                //TODO rumorHandler = new RumorHandler();
                //if (node->getRecvRumors() == 0) {
                //    node->incrementRecvRumors();
                //    for (unsigned int i = 0; i < node->getNeighbors().size(); i++) {
                //        if (node->getNeighbors().at(i).getId() != message.getSenderId()) {
                //            Messages::Message rumorMsg(node->getId(), Messages::MESSAGE_TYPE::APPLICATION, "rumor");
                //            node->sendMessageToNode(&rumorMsg, node->getNeighbors().at(i));
                //        }
                //    }
                //    Messages::Message doneMsg(node->getId(), Messages::MESSAGE_TYPE::CONTROL, "done");
                //    node->sendMessageToNode(&doneMsg, *(node->getInitNode()));
                //    node->setHasSend(true);
                //}
                //node->incrementRecvRumors();
                lock.unlock();
            }else if(msg.find("initiator") != string::npos) {
                //ss << "Neighbor Id: " << node->getId();
                //node->sendToNeighbors(Message(node->getId(), MESSAGE_TYPE::APPLICATION, ss.str()));
                //TODO: EIn Rumor ist APP und eins CTRL
            }else if(msg.find("rumor") != string::npos) {
                // cout << "Done " << node->getId() << "And RECV " << node->getRecvRumors() <<  "And BOOl " << node->getHasSend() << endl;
                for(unsigned int i=0;i< node->getNeighbors().size();i++){
                    //node->sendMessageToNode(Message(node->getId(), MESSAGE_TYPE::APPLICATION,"rumor") , node->getNeighbors().at(i));
                }
                //Message message2(node->getId(), MESSAGE_TYPE::CONTROL, "done");
                //node->sendMessageToNode(message2, *(node->getInitNode()));
                //node->setRecvRumors(node->getConfig().getMinTrust());
                //node->sendToNeighbors(Message(node->getId(), MESSAGE_TYPE::APPLICATION, "rumor"));
            }else if(msg.find("election") != string::npos) {
                echoHandler = new EchoHandler(true, node, node->getConfig().getMaxSend());
            }else if(msg.find("obpl") != string::npos) {
                unique_lock<mutex> lock(goldmanMutex);
                //goldmanEdgeChasingHandler->handleIncommingMessages(msg);
                lock.unlock();
            }else if(msg.find("evaluate") != string::npos){
                string believeS;
                node->getRecvRumors() >= node->getConfig().getMinTrust() ?  believeS = "true" : believeS = "false";
                node->setRecvRumors(0);
                node->setHasSend(false);
                //node->sendMessageToNode(Message (node->getId(), MESSAGE_TYPE::CONTROL, believeS), *(node->getInitNode()));
            }else if(msg.find("end") != string::npos){
                cout << "End Node " << node->getId() << endl;
                exit(EXIT_SUCCESS);
            }
        }
    }else{
        if(msg.find("Node Id: ") != string::npos){
            Messages::Message message(msg);
            cout << "Recv: [ " << message.getCommand() << " ] at ID:" << node->getId() << " " << time << endl;
        }else if(msg.find("balance send") != string::npos ||
                 msg.find("balance request") != string::npos ||
                 msg.find("balance response") != string::npos ||
                 msg.find("request") != string::npos ||
                 msg.find("reply") != string::npos ||
                 msg.find("release") != string::npos){
            unique_lock<mutex> lock(accountMutex);
            accountHandler->handleIncommingMessage(msg, time);
            lock.unlock();
        }
        if(msg.find("echo") != string::npos || msg.find("explorer") != string::npos){
            unique_lock<mutex> lock(echoMutex);
            //TODO: ADD Message for handling
            //echoHandler->handleIncommingMessages(msg);
            lock.unlock();
        }else if(msg.find("vote") != string::npos){
            //TODO:
        }
    }
}



