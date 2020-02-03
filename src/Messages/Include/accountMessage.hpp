//
// Created by alhuber on 02.02.20.
//

#ifndef VAA_ACCOUNTMESSAGE_HPP
#define VAA_ACCOUNTMESSAGE_HPP

#include <ctime>
#include <string>
#include "message.hpp"


namespace Messages{

    class AccountMessage : public Message{
    public:
        //Constructors
        AccountMessage();
        explicit AccountMessage(time_t time);
        explicit AccountMessage(std::string accountMsgAsString);

        AccountMessage(unsigned int i, MESSAGE_TYPE type, const char string[17], unsigned int i1, unsigned int i2);

//Memberfunctions
        std::string toString();

        //Operator
        bool operator< (const AccountMessage& message) const {
            if( message.getSenderId() < this->senderId ||
                message.getOriginId() < this->originId ||
                message.getCommand() <  this->command){
                return true;
            }
        }
    };
}


#endif //VAA_ACCOUNTMESSAGE_HPP
