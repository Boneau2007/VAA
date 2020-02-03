
#ifndef FILE_HANDLER_HPP
#define FILE_HANDLER_HPP

#include <string>
#include <vector>
#include "node.hpp"
#include "edge.hpp"
namespace Graph{
    class Node;
}
namespace Handler {
    class FileHandler{// Classattributes

    private:
        std::string fileName;
        std::vector<Graph::Node> nodeList;

    public:
        // Constructors
        FileHandler();
        FileHandler(std::string fileName);

        // Inline-Elemtfunctions
        std::vector<Graph::Node> getNodeList(){ return nodeList; }

        // Memberfunctions
        void graphgen(const std::string& fileName, const unsigned int nodesNmber, const unsigned int edgeCount);
        void readNodes(unsigned int maxNodesToRead);
        std::vector<Graph::Node> readGraphviz(unsigned int id, const std::string& filePath);
        Graph::Node getNodeFromFile(unsigned int id);
    private:
        static bool maxEdgesPerNode(const std::vector<Graph::Edge>& edgeList, const unsigned int id, unsigned int edgeCount);
        static bool containsEdge(const std::vector<Graph::Edge>& edgeList, const unsigned int idFrom, const unsigned int idTo);
        static void writeGraphvizFile(const std::string& fileName,const std::vector<Graph::Edge>& edgeList);
    };
}
#endif // FILE_HANDLER_HPP




