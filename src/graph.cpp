#include "include/graph.hpp"
Uebung1::Graph::Graph(){}
Uebung1::Graph::Graph(string name){
    setName(name);
}
Uebung1::Graph::Graph(String name, map<unsigned int, node_t> nodeList){
    setName(name);
    setNodeList(nodeList);
}
Uebung1::Graph::Graph(string name, map<unsigned int, node_t> nodeList, vector<edge_t> edgeList){
    setName(name);
    setNodeList(nodeList);
    setEdgeList(edgeList);
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
