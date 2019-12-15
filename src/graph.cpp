#include "include/graph.hpp"

Uebung1::Graph::Graph(string name, vector<Edge> edgeList, map<unsigned int, Uebung1::Node> nodeList){
    this->name = name;
    head = new adjNode*[nodeCount]();
    this->nodeCount = nodeCount;
    for(unsigned int i=0 ;i < nodeCount; i++){
        head[i]=nullptr;
    }
    for(unsigned int i=0;i<edgesCount;i++){
        Uebung1::Node fromNode = edgeList[i].fromNode;
        Uebung1::Node toNode = edgeList[i].toNode;
        insertNode(toNode,head[fromNode]);
    }
}
            
Uebung1::Graph::~Graph(){
    for(unsigned int i=0;i < nodeCount ;i++){
        delete[] head[i];
        delete[] head;
    }
}

void Uebung1::Graph::insertNode(int value, adjNode* head){
    adjNode* newNode = new adjNode;
    newNode->value = value;
    newNode->next = head;
    head = newNode;
}

string Uebung1::Graph::toString(){
    ostringstream stream;
    stream << "Graph " << name << " :" << endl; 
    for(unsigned int i=0; i < nodeCount; i++){
        adjNode* node = head[i];
        while(node != nullptr){
            stream << "(" << i <<  "," << node->value << ")";
            node = node->next;
        }
        stream << endl;
    }
    return stream.str();    
}
