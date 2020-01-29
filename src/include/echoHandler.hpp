//
// Created by alhuber on 26.01.20.
//

#ifndef ECHOHANDLE_HPP
#define ECHOHANDLE_HPP

#include <vector>
#include <mutex>
#include "message.hpp"
#include "node.hpp"
#include "edge.hpp"

namespace Uebung1 {

    enum COLOR {WHITE, RED, GREEN};
    class Node;
    class Message;
    class EchoHandler{

    private:
        // Classattributes
        unsigned int virtualParentId;
        unsigned int maxNodeId;
        unsigned int resultCount;
        std::vector<Uebung1::Edge> spanningTreeList;
        COLOR color;
        bool isWinner;
        bool isIinitiator;
        Uebung1::Node* thisNode;
        unsigned int maxStartNumber;
        unsigned int maxNodes;

    public:
        // Constructors
        EchoHandler();
        EchoHandler(const bool initiator, Uebung1::Node* thisNode, unsigned int maxStartNumber);
        // Deconstructors

        // Inline-Elemtfunctions
        std::vector<Edge> getSpanningTreeList(){ return spanningTreeList; }

        // Memberfunctions
        void forwardExplorer(Uebung1::Message* message);
        void sendEcho();
        void executeStartElection();
        void sendStartDoubleCounting();
        void handleIncommingMessages(Uebung1::Message* message);
    };
}

#endif //ECHOHANDLE_HPP
