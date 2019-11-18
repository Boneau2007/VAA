
#include "node.hpp"

Uebung1::Node::Node(unsigned int id, string ipAddress, unsigned int port){
    setId(id);
    setIpAddress(ipAddress);
    setPort(port);
}


Uebung1::Node Uebung1::Node::getNeighbor(unsigned int id){
    return getNeighbors().at(id);
}
      
void Uebung1::Node::addNeighbor(Node neighbor){
    //TODO: Fehlerbehandlung
    getNeighbors().insert(neighbor.getId(), neighbor);
}

void Uebung1::Node::removeNeighbor(unsigned int id){
    //TODO: Fehlerbehandlung
    getNeighbors().erase(id);
}


void Uebung1::Node::updateNeighbor(unsigned int id, Node neighbor){
    //TODO: Fehlerbehandlung
    getNeighbors().at(id) = neighbor;
}
