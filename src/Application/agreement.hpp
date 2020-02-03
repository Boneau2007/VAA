//
// Created by alhuber on 29.01.20.
//

#ifndef VAA_AGREEMENT_HPP
#define VAA_AGREEMENT_HPP

#include "../Messages/IMessage.hpp"
#include "../Graph/node.hpp"

namespace Application{
    class Message;
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
