//
// Created by alhuber on 29.01.20.
//

#ifndef VAA_AGREEMENT_HPP
#define VAA_AGREEMENT_HPP

#include "IMessage.hpp"
#include "node.hpp"

namespace Graph{
    class Node;
}

namespace Application{
    class Message;
    class Agreement {
        private:
            Graph::Node* thisNode;
    public:
        Agreement();
        Agreement(Graph::Node* node);
        void handleIncommingMessages(Message* pMessage);
    };


}


#endif //VAA_AGREEMENT_HPP
