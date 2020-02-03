//
// Created by alhuber on 02.02.20.
//

#include "Include/accountMessage.hpp"

using namespace std;
using namespace Messages;

AccountMessage::AccountMessage() : Message() {}

AccountMessage::AccountMessage(time_t time) : Message() {
    this->time = time;
}

AccountMessage::AccountMessage(string accountMsgAsString) : Message(){

}

std::string AccountMessage::toString() {

    return Message::toString();
}

AccountMessage::AccountMessage(unsigned int i, MESSAGE_TYPE type, const char *string, unsigned int i1,
                               unsigned int i2) {

}
