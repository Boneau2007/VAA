//
// Created by alhuber on 02.02.20.
//

#ifndef VAA_GOLDMANEDGECHASINGHANDLER_HPP
#define VAA_GOLDMANEDGECHASINGHANDLER_HPP

#include <vector>
#include "node.hpp"
#include "orderedBlockedMessage.hpp"
namespace Graph{
    class Node;
}
namespace Handler{
    class GoldmanEdgeChasingHandler{
    private:
        Graph::Node* node;

    public:
        //Contructors
        GoldmanEdgeChasingHandler();
        explicit GoldmanEdgeChasingHandler(Graph::Node *node);

        //Inline-Memberfunction
        //Memberfunctions
        void handleIncommingMessages(Messages::OrderedBlockedMessage* message);

        void initiateOBPL(std::vector<unsigned int> nodeIdList);

        void sendExtendOBPL();
    };

}


#endif //VAA_GOLDMANEDGECHASINGHANDLER_HPP
