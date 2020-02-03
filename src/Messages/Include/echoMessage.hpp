//
// Created by alhuber on 02.02.20.
//

#ifndef VAA_ECHOMESSAGE_HPP
#define VAA_ECHOMESSAGE_HPP

#include <vector>
#include <string>
#include "edge.hpp"
#include "forwardMessage.hpp"

namespace Messages{
    class EchoMessage : public ForwardMessage {
    public:
        EchoMessage();
        EchoMessage(const std::string& echoMsgAsString);
        EchoMessage(unsigned int senderId, Messages::MESSAGE_TYPE type, std::string  content, unsigned int originId,
                std::vector<Graph::Edge> hopList);


        //Inline-Elementfunctions

        //Memberfunctions
        std::string toString();
    };



}


#endif //VAA_ECHOMESSAGE_HPP
