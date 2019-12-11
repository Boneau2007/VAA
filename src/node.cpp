#include "include/node.hpp"

Uebung1::Node::Node(unsigned int id, string ipAddress, unsigned int port, bool initiator){
    setId(id);
    setIpAddress(ipAddress);
    setPort(port);
    setInitiator(initiator);
}

Uebung1::Node::Node(unsigned int id, string ipAddress, unsigned int port){
    setId(id);
    setIpAddress(ipAddress);
    setPort(port);
}
Uebung1::Node::Node(){}

Uebung1::Node Uebung1::Node::getNeighbor(unsigned int id){
    return getNeighbors().at(id);
}
      
void Uebung1::Node::addNeighbor(Node neighbor){
    //TODO: Fehlerbehandlung
    neighborList.insert(make_pair(neighbor.getId(), neighbor));
}

void Uebung1::Node::removeNeighbor(unsigned int id){
    //TODO: Fehlerbehandlung
    neighborList.erase(id);
}


void Uebung1::Node::updateNeighbor(unsigned int id, Node neighbor){
    //TODO: Fehlerbehandlung
    neighborList.at(id) = neighbor;
}
