//
// Created by alhuber on 02.02.20.
//

#include "forwardMessage.hpp"

Messages::ForwardMessage::ForwardMessage() {

}

Messages::ForwardMessage::ForwardMessage(unsigned int senderId, Messages::MESSAGE_TYPE type) : Message(senderId, type) {

}

Messages::ForwardMessage::ForwardMessage(unsigned int senderId, Messages::MESSAGE_TYPE type, std::string content)
        : Message(senderId, type, content) {

}

Messages::ForwardMessage::ForwardMessage(const std::string &content) : Message(content) {

}
