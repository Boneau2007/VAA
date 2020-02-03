//
// Created by alhuber on 02.02.20.
//

#ifndef VAA_ORDEREDBLOCKEDMESSAGE_HPP
#define VAA_ORDEREDBLOCKEDMESSAGE_HPP

#include <vector>
#include <string>
#include "message.hpp"

namespace Messages{

    class OrderedBlockedMessage : public Message{
    public:
        //Constructors
        OrderedBlockedMessage();
        explicit OrderedBlockedMessage(std::string orderedBlockedMessageString);

        //Memberfunctions
        std::string toString();
    };
}

#endif //VAA_ORDEREDBLOCKEDMESSAGE_HPP
