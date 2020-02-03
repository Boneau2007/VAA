//
// Created by alhuber on 26.01.20.
//

#ifndef ECHOHANDLE_HPP
#define ECHOHANDLE_HPP

#include <vector>
#include <mutex>
#include "../Messages/echoMessage.hpp"
#include "../Graph/node.hpp"
#include "../Graph/edge.hpp"

namespace Handler{

    enum COLOR {WHITE, RED, GREEN};
    class Node;
    class Message;
    class EchoHandler{

    private:
        // Classattributes
        unsigned int virtualParentId;
        unsigned int maxNodeId;
        unsigned int resultCount;
        std::vector<Graph::Edge> spanningTreeList;
        COLOR color;
        bool isWinner;
        bool isIinitiator;
        Node* thisNode;
        unsigned int maxStartNumber;
        unsigned int maxNodes;

    public:
        // Constructors
        EchoHandler();
        EchoHandler(const bool initiator, Node* thisNode, unsigned int maxStartNumber);
        // Deconstructors

        // Inline-Elemtfunctions
        std::vector<Graph::Edge> getSpanningTreeList(){ return spanningTreeList; }

        // Memberfunctions
        void forwardExplorer(Messages::EchoMessage* message);
        void sendEcho();
        void executeStartElection();
        void sendStartDoubleCounting();
        void handleIncommingMessages(Messages::EchoMessage* message);
    };
}

#endif //ECHOHANDLE_HPP
