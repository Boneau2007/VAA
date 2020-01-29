
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
            std::string fileName;
            std::vector<Node> nodeList;

        public:
            // Constructors
            FileHandler();
            FileHandler(std::string fileName);
            // Deconstructors

            // Inline-Elemtfunctions
            //std::string getFileName(){ return fileName; }
            //void setFileName(std::string fileName){ this->fileName = fileName; }

            std::vector<Node> getNodeList(){ return nodeList; }

            // Memberfunctions
            void graphgen(const std::string& fileName, const unsigned int nodesNmber, const unsigned int edgeCount);
            void readNodes(unsigned int maxNodesToRead);
            std::vector<Uebung1::Node> readGraphviz(unsigned int id, const std::string& filePath);
            Uebung1::Node getNodeFromFile(unsigned int id);

        private:
            static bool maxEdgesPerNode(const std::vector<Uebung1::Edge>& edgeList, const unsigned int id, unsigned int edgeCount);
            static bool containsEdge(const std::vector<Uebung1::Edge>& edgeList, const unsigned int idFrom, const unsigned int idTo);
            static void writeGraphvizFile(const std::string& fileName,const std::vector<Uebung1::Edge>& edgeList);
    };
}
#endif // FILE_HANDLER_HPP




