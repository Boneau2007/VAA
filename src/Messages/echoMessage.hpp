//
// Created by alhuber on 02.02.20.
//

#ifndef VAA_ECHOMESSAGE_HPP
#define VAA_ECHOMESSAGE_HPP

#include <vector>
#include <string>
#include "../Graph/edge.hpp"
#include "forwardMessage.hpp"

namespace Messages{
    class EchoMessage : public ForwardMessage {
    protected:
        std::vector<Graph::Edge> hopList;

    public:
        EchoMessage();
        EchoMessage(const std::string& echoMsgAsString);
        EchoMessage(unsigned int senderId, Messages::MESSAGE_TYPE type, std::string  content, unsigned int originId,
                std::vector<Graph::Edge> hopList);


        //Inline-Elementfunctions
        std::vector<Graph::Edge> getHopList() const { return hopList; }
        void setHopList(std::vector<Graph::Edge> hopList){ this->hopList = hopList; }

        //Memberfunctions
        std::string toString();
    };



}


#endif //VAA_ECHOMESSAGE_HPP
