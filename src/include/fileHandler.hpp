
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
            std::vector<Uebung1::Node> nodeList;
            
        public:
            // Constructors
            FileHandler();
            FileHandler(const std::string fileName);
            FileHandler(const std::string nodeFile, const std::string graphFile);
            // Deconstructors
            
            // Inline-Elemtfunctions
            std::string getFileName(){ return nodeFile; }
            void setFileName(std::string nodeFile){ this->nodeFile = nodeFile; }

            std::string getGraphFile(){ return graphFile; }
            void setGraphFile(std::string graphFile){ this->graphFile = graphFile; }

            std::vector<Uebung1::Node> getNodeList(){ return nodeList; }
            void setNodeList(std::vector<Uebung1::Node> nodeList){ this->nodeList = nodeList; }

            // Memberfunctions
            void graphgen(const std::string fileName, const unsigned int edgeCount);
            std::vector<Uebung1::Node> readNodes();
            std::vector<Uebung1::Node> readGraphviz(const unsigned int id);
            Uebung1::Node getNodeFromFile(const unsigned int id);
        private:
            bool containsEdge(const std::vector<Uebung1::Edge> edgeList, const unsigned int idFrom, const unsigned int idTo);
            void writeGraphvizFile(const std::string fileName,const std::vector<Uebung1::Edge> edgeList);
    };
}
#endif // FILE_HANDLER_HPP




