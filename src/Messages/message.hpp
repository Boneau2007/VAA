//
// Created by alhuber on 02.02.20.
//

#ifndef VAA_MESSAGEIMPL_HPP
#define VAA_MESSAGEIMPL_HPP

#include <string>
#include "IMessage.hpp"

namespace Messages{

    class Message : public IMessage {

    public:
        // Constructors
        Message();
        Message(unsigned int senderId, MESSAGE_TYPE type);
        Message(unsigned int senderId, MESSAGE_TYPE type, std::string  content);
        explicit Message(const std::string& content);

        // Memberfunctions
        std::string toString();
    };
}


#endif //VAA_MESSAGEIMPL_HPP
