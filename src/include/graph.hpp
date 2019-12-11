
#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <string>
#include <sstream> 

using namespace std;

namespace Uebung1 {
    
    struct adjNode {
        unsigned int value;
        adjNode* next;
    };

    struct edge {
        unsigned int fromNode, toNode;
    };

    class Graph{
        private:
            string name;
            adjNode** head;
            unsigned int nodeCount;
        public:
            Graph(string name, edge edges[], unsigned int edgesCount, unsigned int nodeCount);
            ~Graph();
            adjNode** getHead(){ return head; }
            unsigned int getNodeCount(){ return nodeCount; }
            string toString();
        private:
            void insertNode(unsigned int value, adjNode* head);
    };
}
#endif // GRAPH_HPP