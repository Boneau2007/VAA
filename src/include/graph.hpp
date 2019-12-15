
#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <string>
#include <sstream> 

using namespace std;

namespace Uebung1 {
    
    struct adjNode {
        node value; 
        adjNode* next;
    };

    struct edge {
       node fromNode, toNode;
    }edge_t;

    struct node {
        unsigned int id;
        string ipAddress;
        unsigned int port;
        bool initiator;
    }node_t;

    class Graph{
        private:
            string name;
            adjNode** head;
            map<unsigned int, node_t> nodeList;
            vector<edge_t> edgeList;
           
        public:
            Graph(string name);
            Graph(string name, map<unsigned int, node_t> nodeList);
            Graph(string name, map<unsigned int, node_t> nodeList, vector<edge_t> edgeList);
            ~Graph();
    
            void addNode(node_t node);
            void removeNode(unsigned int nodeId);
            void updateNode(unsigned int nodeId, node_t node);
            
            void addEdge(edge_t edge);
            void removeEdge(unsigned int edgeListItemId);
            void updateEdge(unsigned int edgeListItemId, edge_t edge);

            void setNodeList(map<unsigned int, node_t> nodeList){ this->nodeList = nodeList; }
            map<unsigned int, node_t> getNodeList(){ return nodeList; }
            void setEdgeList(vector<edge_t> edgeList){ this->edgeList = edgeList; }
            vector<edge_t> getEdgeList(){ return edgeList; }

            adjNode** getHead(){ return head; }
            string toString();
        private:
            void insertNode(node_t value, adjNode* head);
    };
}
#endif // GRAPH_HPP
