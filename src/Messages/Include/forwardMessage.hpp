//
// Created by alhuber on 02.02.20.
//

#ifndef VAA_FORWARDEDMESSAGE_HPP
#define VAA_FORWARDEDMESSAGE_HPP

#include <string>
#include "message.hpp"

namespace Messages{

    class ForwardMessage : public Message{
    public:
        ForwardMessage();
        ForwardMessage(unsigned int senderId, MESSAGE_TYPE type);
        ForwardMessage(unsigned int senderId, MESSAGE_TYPE type, std::string  content);
        explicit ForwardMessage(const std::string& content);

        //Memeberfunction
        std::string toString();
    };
}


#endif //VAA_FORWARDEDMESSAGE_HPP
