//
// Created by alhuber on 29.01.20.
//

#ifndef VAA_AGREEMENT_HPP
#define VAA_AGREEMENT_HPP

#include "message.hpp"
#include "node.hpp"

namespace Uebung1{
    class Agreement {
        private:
            Node* thisNode;
    public:
        Agreement();
        Agreement(Node* node);
        void handleIncommingMessages(Message* pMessage);
    };


}


#endif //VAA_AGREEMENT_HPP
