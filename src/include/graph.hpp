
#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <string>
#include <sstream> 

using namespace std;

namespace Uebung1 {
    
    struct adjNode {
        int value;
        adjNode* next;
    };

    struct edge {
       node fromNode, toNode;
    };

    struct node {
        unsigned int id;
        string ipAddress;
        unsigned int port;
    };

    class Graph{
        private:
            string name;
            adjNode** head;
            map<unsigned int, node> nodeList;
            vector<edge> edgeList;
           
        public:
            Graph(string name, vector<edge> edgeList, <unsigned int, Uebung1::Node> nodeList);
            ~Graph();
            adjNode** getHead(){ return head; }
            string toString();
        private:
            void insertNode(Uebung1::Node value, adjNode* head);
    };
}
#endif // GRAPH_HPP
