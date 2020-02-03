//
// Created by alhuber on 02.02.20.
//

#ifndef VAA_GOLDMANEDGECHASINGHANDLER_HPP
#define VAA_GOLDMANEDGECHASINGHANDLER_HPP

#include <vector>
#include "../Graph/node.hpp"

namespace Handler{
    class Node;
    class GoldmanEdgeChasingHandler{
    private:
        Node* node;

    public:
        //Contructors
        GoldmanEdgeChasingHandler();
        explicit GoldmanEdgeChasingHandler(Node *node);

        //Inline-Memberfunction
        //Memberfunctions
        void handleIncommingMessages(Message *message);

        void initiateOBPL(std::vector<unsigned int> nodeIdList);

        void sendExtendOBPL();
    };

}


#endif //VAA_GOLDMANEDGECHASINGHANDLER_HPP
