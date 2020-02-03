//
// Created by alhuber on 02.02.20.
//

#ifndef VAA_FORWARDEDMESSAGE_HPP
#define VAA_FORWARDEDMESSAGE_HPP

#include <string>
#include "message.hpp"
namespace Messages{

    class ForwardMessage : public Message{
    private:
        unsigned int originId;

    public:

        ForwardMessage();
        ForwardMessage(unsigned int senderId, MESSAGE_TYPE type);
        ForwardMessage(unsigned int senderId, MESSAGE_TYPE type, std::string  content);
        explicit ForwardMessage(const std::string& content);

        //Inline-Elementfunctions
        unsigned int getOriginId() const { return originId; }
        void setOriginId(unsigned int originId){ this->originId = originId; }

        //Memeberfunction
        std::string toString();
    };
}


#endif //VAA_FORWARDEDMESSAGE_HPP
