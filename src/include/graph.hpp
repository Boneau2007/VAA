
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
    };

    struct node {
        unsigned int id;
        string ipAddress;
        unsigned int port;
        bool initiator;
        vector<node> neighbourList;
    };

    class Graph{
        private:
            string name;
            adjNode** head;
            map<unsigned int, node> nodeList;
            vector<edge> edgeList;
           
        public:
            Graph();
            Graph(string name);
            Graph(string name, map<unsigned int, node> nodeList);
            Graph(string name, map<unsigned int, node> nodeList, vector<edge> edgeList);
            ~Graph();
    
            void addNode(node node);
            void removeNode(unsigned int);
            void updateNode(unsigned int, node node);
            adjNode** getHead(){ return head; }
            string toString();
        private:
            void insertNode(Uebung1::Node value, adjNode* head);
    };
}
#endif // GRAPH_HPP
