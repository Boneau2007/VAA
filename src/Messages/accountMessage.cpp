//
// Created by alhuber on 02.02.20.
//

#include "accountMessage.hpp"

using namespace std;
using namespace Messages;

AccountMessage::AccountMessage() : Message(), time(0){}

AccountMessage::AccountMessage(time_t time) : Message(), time(time){}

AccountMessage::AccountMessage(string accountMsgAsString) : Message(), time(time){}

std::string AccountMessage::toString() {

    return Message::toString();
}
