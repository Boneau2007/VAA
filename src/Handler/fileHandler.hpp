
#ifndef FILE_HANDLER_HPP
#define FILE_HANDLER_HPP

#include <string>
#include <vector>
#include "../Graph/node.hpp"
#include "../Graph/edge.hpp"

namespace Handler {
    class Node;
    class FileHandler{// Classattributes

    private:
        std::string fileName;
        std::vector<Node> nodeList;

    public:
        // Constructors
        FileHandler();
        FileHandler(std::string fileName);

        // Inline-Elemtfunctions
        std::vector<Node> getNodeList(){ return nodeList; }

        // Memberfunctions
        void graphgen(const std::string& fileName, const unsigned int nodesNmber, const unsigned int edgeCount);
        void readNodes(unsigned int maxNodesToRead);
        std::vector<Node> readGraphviz(unsigned int id, const std::string& filePath);
        Node getNodeFromFile(unsigned int id);
    private:
        static bool maxEdgesPerNode(const std::vector<Graph::Edge>& edgeList, const unsigned int id, unsigned int edgeCount);
        static bool containsEdge(const std::vector<Graph::Edge>& edgeList, const unsigned int idFrom, const unsigned int idTo);
        static void writeGraphvizFile(const std::string& fileName,const std::vector<Graph::Edge>& edgeList);
    };
}
#endif // FILE_HANDLER_HPP




