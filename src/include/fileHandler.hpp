
#ifndef FILE_HANDLER_HPP
#define FILE_HANDLER_HPP

#include <string>
#include <vector>
#include "node.hpp"
#include "edge.hpp"

namespace Uebung1 {
    class Node;
    class FileHandler{
        // Classattributes
        private:
            std::string nodeFile;
            std::string graphFile;
            
        public:
            // Constructors
            FileHandler();
            FileHandler(const std::string& fileName);
            FileHandler(const std::string& nodeFile, const std::string& graphFile);
            // Deconstructors
            
            // Inline-Elemtfunctions
            std::string getFileName(){ return nodeFile; }
            void setFileName(std::string nodeFile){ this->nodeFile = nodeFile; }

            std::string getGraphFile(){ return graphFile; }
            void setGraphFile(std::string graphFile){ this->graphFile = graphFile; }

            // Memberfunctions
            void graphgen(const std::string fileName, std::vector<Uebung1::Node> nodeList, const unsigned int nodesNmber, const unsigned int edgeCount);
            std::vector<Uebung1::Node> readNodes(const unsigned int maxNodesToRead);
            std::vector<Uebung1::Node> readGraphviz(const unsigned int id, const std::vector<Uebung1::Node>& nodeList);
            Uebung1::Node getNodeFromFile(const unsigned int id, std::vector<Uebung1::Node> nodeList);
        private:
            bool maxEdgesPerNode(const std::vector<Uebung1::Edge> edgeList, const unsigned int id, unsigned int edgeCount);
            bool containsEdge(const std::vector<Uebung1::Edge> edgeList, const unsigned int idFrom, const unsigned int idTo);
            void writeGraphvizFile(const std::string fileName,const std::vector<Uebung1::Edge> edgeList);
    };
}
#endif // FILE_HANDLER_HPP




