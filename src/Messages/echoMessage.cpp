//
// Created by alhuber on 02.02.20.
//

#include "echoMessage.hpp"

using namespace std;
using namespace Graph;
using namespace Messages;

EchoMessage::EchoMessage() {}

EchoMessage::EchoMessage(unsigned int senderId, Messages::MESSAGE_TYPE type, std::string content,
                                   unsigned int originId, std::vector<Edge> hopList) : ForwardMessage(senderId, type, content,
                                                                                               originId, hopList) {

}


EchoMessage::EchoMessage(const std::string &echoMsgAsString) {

}
