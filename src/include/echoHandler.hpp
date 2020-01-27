//
// Created by alhuber on 26.01.20.
//

#ifndef VAA_ECHOHANDLE_HPP
#define VAA_ECHOHANDLE_HPP

#include <vector>
#include "message.hpp"
#include "node.hpp"

namespace Uebung1 {

    enum COLOR {WHITE, RED, GREEN};

    class EchoHandler{

    private:
        // Classattributes
        unsigned int virtualParentId;
        unsigned int maxNodeId;
        unsigned int resultCount;
        std::vector<unsigned int> spanningTreeNodeIdList;
        COLOR color;
        bool isWinner;
        bool isIinitiator;
        Uebung1::Node* thisNode;
        unsigned int maxStartNumber;

    public:
        // Constructors
        EchoHandler();
        EchoHandler(const bool initiator, Uebung1::Node* thisNode, const unsigned int maxStartNumber);
        // Deconstructors

        // Inline-Elemtfunctions

        // Memberfunctions
        void forwardExplorer(Uebung1::Message message);
        void sendEcho();
        void startElection();
        void sendStartDoubleCounting(unsigned int maxNumber);
        void handleIncommingMessages(Uebung1::Message message);
    };
}

#endif //VAA_ECHOHANDLE_HPP
