//
// Created by alhuber on 26.01.20.
//

#include "echoHandler.hpp"

using namespace std;
using namespace Uebung1;

EchoHandler::EchoHandler(const bool initiator, Node* thisNode, const unsigned int maxStartNumber)
    : virtualParentId(0)/*evtl Node*/, maxNodeId(0), resultCount(0), color(COLOR::WHITE), isWinner(false),
        isIinitiator(initiator), thisNode(thisNode), maxStartNumber(maxStartNumber) {}

void EchoHandler::handleIncommingMessages(Message message){
    if(color != COLOR.GREEN){
        if(message.getOriginId() == maxNodeId){
            resultCount++;
            if(message.getContent() == "echo"){
                for(auto i : message.getSpanningTree()){
                    spanningTreeNodeIdList.push_back(i);
                }
            }
            if(color == COLOR.WHITE){
                color = COLOR.RED;
                forwardExplorer(message);
                virtualParentId = message.getSenderId();
            }
            if(resultCount == thisNode->getNeighbors().size() && color != COLOR.GREEN){
                color == COLOR.GREEN;
                if(thisNode->getId() == message.getOriginId()){
                    thisNode.setCoordinator(thisNode);
                    sleep(1);
                    Message msg(thisNode->getId(),"ctrl","won election");
                    thisNode->sendToSuperNode(msg);
                    sendStartDoubleCounting();
                    thisNode->startDoubleCounting();
                }else{
                    sendEcho();
                }
            }
        }else if(message.getOriginId() > virtualParentId){
            resultCount=1;
            maxNodeId = message.getOriginId();
            virtualParentId = message.getSenderId();
            forwardExplorer(message);
            spanningTreeNodeIdList.clear();
        }

    }
}

void EchoHandler::forwardExplorer(Message message){
    message.setSenderId(virtualParentId);
    for(unsigned int i=0;i< thisNode->getNeighbors().size(); i++){
        if(node->getNeighbors().at(i).getId() != virtualParentId){
            sendMessage(message , node->getNeighbors().at(i));
        }
    }
}
void EchoHandler::sendEcho(){


    for(unsigned int i=0;i < thisNode->getNeighbors().size(); i++){
        ss << node->getEchoNeighbors().at(i).toString() << ";";
    }
    Message message(node->getId(), MESSAGE_TYPE::APPLICATION, ss.str());
    for(unsigned int i=0;i< node->getNeighbors().size();i++){
        if(node->getNeighbors().at(i).getId() == senderId){
            sendMessage(message , node->getNeighbors().at(i));
        }
    }
}

void EchoHandler::startElection(){
}


void EchoHandler::sendStartDoubleCounting(unsigned int maxNumber){
    for(unsigned int i : maxNumber){
        unsigned int num = rand()%nodeList.size();
        thisNode->sendToNode(Message(thisNode->getId(), "app", "vote"), nodeList.at(num));
        nodeList.erase(num);
}


    stringstream ss;
    ss << message.getContent();
    char delemiter(';');
    string command;
    getline(ss, command , delemiter);
    unsigned int recvVirtualId = atoi(command.substr(5, command.size()).c_str());
    if(node->getVirtualParentId() == recvVirtualId){
        cout << "ID: " << node->getId() << " Recv Echo From: "<< message.getSenderId() << "Done" << message.getSenderId()  << endl;
        string contentSubString = message.getContent().substr(command.size(), message.getContent().size());
        node->addEchoNeighborMsg(contentSubString);
        for(unsigned int i=0; i < node->getNeighbors().size(); i++){
            if(node->getNeighbors().at(i).getId() == message.getSenderId()){
                node->getNeighbors().at(i).setDone(true);
                break;
            }
        }
        cout << "ID on echo: " << node->getId() << "Was Last Edge: "<< boolalpha << wasLastNotVirtualNodeEdge()  << " Is INIT: "<< node->getInitiator()<< endl;
        if(wasLastNotVirtualNodeEdge() && !node->getInitiator()){
            cout << "ID: " << node->getId() << " send Echo Back to virtualId: "<< node->getVirtualParentId() << endl;
            sendEcho(node->getId(),node->getVirtualParentId());
        }else if(wasLastNotVirtualNodeEdge() && node->getInitiator()){
            cout << "ID: " << node->getId() << "start Voteing" << endl;
            node->setWinner(true);
            startUnificationProcess();
        }
    }
}else if(message.getContent() == "election"){
stringstream ss;
ss << "vote " << node->getPreferedTime();
Message messageOut(node->getId(), MESSAGE_TYPE::APPLICATION, ss.str());
// Wählt p random nachbarn
vector<Node> randNeighbors = getRandNodeIdList(node->getMaxPhilosopherNumber());
for(auto & randNeighbor : randNeighbors){
sendMessage(messageOut , randNeighbor);
}
else if(message.getContent().compare(0,8,"explorer") == 0){
unsigned int recvVirtualId = atoi(message.getContent().substr(9, message.getContent().size()).c_str());
cout << "ID: " << node->getId() << " Recv VirtualId: " << recvVirtualId << " From: "<< message.getSenderId() <<" Had VirtualId: " << node->getVirtualParentId() << endl;
// Grad == 1, hat keine Nachbarn
if(node->getNeighbors().size() == 1){
cout << "ID: " << node->getId() << "Send Echo to: " << message.getSenderId() << endl;
sendEcho(message.getSenderId(), recvVirtualId);
// Grad > 1, hat Nachbarn
}else{
// j > i Knoten wird von stärkerem Initiator erobert
if(recvVirtualId > node->getVirtualParentId()){
cout << "ID: " << node->getId() << " send Explorer" << endl;
node->setVirtualParentId(recvVirtualId);
sendExplorer(message.getSenderId());
// j == i, d.h. zwei Explorer identischer Markierung begegnen sich
}else if(recvVirtualId == node->getVirtualParentId()){
cout << "ID: " << node->getId() << " recvNode Done "<< message.getSenderId() << endl;
for(unsigned int i=0; i < node->getNeighbors().size(); i++){
if(node->getNeighbors().at(i).getId() == message.getSenderId()){
node->getNeighbors().at(i).setDone(true);
break;
}
}
cout << "ID on explorer: " << node->getId()  << "Was Last Edge: "<< boolalpha << wasLastNotVirtualNodeEdge()  << " Is INIT: "<< node->getInitiator()<< endl;
if(wasLastNotVirtualNodeEdge()){
cout << "ID: " << node->getId() << " send Echo Back to virtualId: "<< node->getVirtualParentId() << endl;
sendEcho(node->getId(), node->getVirtualParentId());
}
}
}else if(message.getContent().compare(0,4, "vote") == 0){
if(node->getMaxPhilosopherNumber() > node->getVoteCount()){
time_t recvTime = atoi(message.getContent().substr(5, message.getContent().size()).c_str());
stringstream ss;
ss << "back " << node->getPreferedTime();
Message messageOut(node->getId(), MESSAGE_TYPE::APPLICATION, ss.str());
for(unsigned int i=0; i < node->getNeighbors().size(); i++){
if(node->getNeighbors().at(i).getId() == message.getSenderId()){
sendMessage(messageOut , node->getNeighbors().at(i));
break;
}
}
//Mittelwertberechnung
time_t diff = (recvTime + node->getPreferedTime()) >> 2;
// Aufrunden des Mittelwerts wenn dieser ungerade war
if((recvTime + node->getPreferedTime()) % 2 != 0){
diff++;
}
node->setPreferedTime(diff);
node->incrementVoteCount();
}
