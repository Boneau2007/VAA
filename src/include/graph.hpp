
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
        vector<node_t> neighbourList;
    }node_t;

    class Graph{
        private:
            string name;
            adjNode** head;
            map<unsigned int, node_t> nodeList;
            vector<edge_t> edgeList;
           
        public:
            Graph();
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

            adjNode** getHead(){ return head; }
            string toString();
        private:
            void insertNode(node_t value, adjNode* head);
    };
}
#endif // GRAPH_HPP
