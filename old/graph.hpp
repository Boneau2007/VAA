
#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <vector>  
#include <algorithm>
#include "node.hpp"
#include "edge.hpp"

namespace Uebung1 {

    typedef std::vector<Uebung1::Edge*> adjList_t;
    typedef std::vector<Uebung1::Node*> nodeList_t;

    class Graph{

        // Classattributes
        private:
            std::string name;
            nodeList_t nodeList;
            adjList_t adjList;
           
        public:
            // Constructors
            Graph();
            Graph(const std::string name);
            Graph(const nodeList_t nodeList);
            Graph(const std::string name, const nodeList_t nodeList);
            Graph(const nodeList_t nodeList, const adjList_t adjList);
            Graph(const std::string name, const nodeList_t nodeList, const adjList_t adjList);
           
            // Deconstructors
            //~Graph();
            
            // Inline-Elemtfunctions
            std::string getName() const { return name; }
            void setName(std::string name){ this->name = name; }

            nodeList_t getNodeList() const { return nodeList; }
            void setNodeList(nodeList_t nodeList){ this->nodeList = nodeList; }

            adjList_t getAdjList() const{ return adjList; }
            void setAdjList(adjList_t adjList){ this->adjList = adjList; }

            // Memberfunctions
            void addNode(Uebung1::Node* node);
            void removeNode(Uebung1::Node& node);
            bool contains(const Uebung1::Node& node);
            Node* getNode(unsigned int nodeId);
            
            void addEdge(Uebung1::Node* fromNode, Uebung1::Node* toNode, unsigned int value);
            void removeEdge(Uebung1::Edge* edge);
            bool contains(const Uebung1::Node& fromNode, const Uebung1::Node& toNode);

            std::string toString();
            
    };
}
#endif // GRAPH_HPP
