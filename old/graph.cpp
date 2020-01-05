#include <sstream>
#include "graph.hpp"

using namespace std;
using namespace Uebung1;

Graph::Graph() : name("") {}
Graph::Graph(const string name) : name(name) {}

Graph::Graph(const nodeList_t nodeList) : nodeList(nodeList) {}

Graph::Graph(const string name, const nodeList_t nodeList) 
    : name(name), nodeList(nodeList) {}

Graph::Graph(const nodeList_t nodeList, const adjList_t adjList)
    : nodeList(nodeList), adjList(adjList) {}

void Graph::addNode(Node* node){
    if(contains(*node)){
        throw  runtime_error("Node already in graph");
    }
    nodeList.push_back(node);
}

void Graph::removeNode(Node& node){
    if(!contains(node)){
        throw  runtime_error("Node not in graph");
    }
    for(unsigned int i=0;i < nodeList.size(); i++){
        Node* nodeToRemove = nodeList.at(i);
        if(node.getId() == nodeToRemove->getId()){                         // Get Node Index
            for(unsigned int neighborIndex=0;neighborIndex < nodeToRemove->getNeighbors().size(); neighborIndex++){     
                Node* neighborToModifi = nodeToRemove->getNeighbors().at(neighborIndex);  // Do for Nodes Neighborlist     
                for(unsigned int j=0;j < nodeList.size(); j++){                      // Find the NeighborNode in the NodeList , j = neighborInNodeListIndex
                Node* neighborToModifiInList = nodeList.at(j);
                    if(neighborToModifiInList->getId() == neighborToModifi->getId()){
                        for(unsigned int k=0; k < neighborToModifiInList->getNeighbors().size();k++)  {// nodeToRemoveInNeighbor
                            Node* neighborToRemoveInNeighbor = neighborToModifiInList->getNeighbors().at(k); 
                            if(neighborToRemoveInNeighbor->getId() == node.getId()){
                                nodeList.at(j)->getNeighbors().erase(nodeList.at(j)->getNeighbors().begin() + k);
                            }
                        }                              
                    }
                }
            }
            nodeList.erase(nodeList.begin()+i);
            return ;
        }
    }
    throw  runtime_error("NodeId not contained in list");
}

void Graph::addEdge(Node* fromNode, Node* toNode, unsigned int value){
    if(fromNode->getId() == toNode->getId()){
        throw  runtime_error("ERROR: Identical ID");
    }
    if(contains(*fromNode, *toNode)){
        throw  runtime_error("ERROR: Edge already exists");
    }
    Edge* edge = new Edge(fromNode, toNode, value);
    edge->toString();
    adjList.push_back(edge);
}

void Graph::removeEdge(Edge* edge){
     for(unsigned int i=0;i < nodeList.size(); i++){
         Node* currentNode = nodeList.at(i);
         for(unsigned int j=0;j < currentNode->getNeighbors().size();j++){
         Node* currentNeighbor = currentNode->getNeighbors().at(i);
            if(currentNeighbor->getId() == edge->getFromNode()->getId() || 
               currentNeighbor->getId() == edge->getToNode()->getId()) {
                   currentNode->getNeighbors().erase(currentNode->getNeighbors().begin() + j);
        }
        }
    }
}

// O(n)
bool Graph::contains(const Node& node){
    for(unsigned int i=0;i < nodeList.size(); i++){
        if(node.getId() == nodeList.at(i)->getId()){
            return true;
        }
    }
    return false;
}

bool Graph::contains(const Node& fromNode, const Node& toNode){
    for(unsigned int i=0;i < adjList.size(); i++){
        Edge& edge = *adjList.at(i);
        if(fromNode.getId() == edge.getFromNode()->getId() && 
           toNode.getId()   == edge.getToNode()->getId()){
            return true;
        }
    }
    return false;
}

string Graph::toString(){
    ostringstream stream;
    if(name.empty()){
        name = "G";
    }
    stream << "graph " << name << " {" << endl; 
    for(unsigned int i=0; i < adjList.size(); i++){
        stream << adjList.at(i)->toString();
    }
    stream << "}";
    return stream.str();    
}

