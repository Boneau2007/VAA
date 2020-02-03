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

    protected:
        //Classattribute
        time_t time;

    public:
        //Constructors
        AccountMessage();
        explicit AccountMessage(time_t time);
        explicit AccountMessage(std::string accountMsgAsString);

        //Inline-Elementfunctions
        time_t getTimestamp() const { return time; }
        //void setTimestamp(time_t time){ this->time = time; }

        //Memberfunctions
        std::string toString();

        //Operator
        bool operator< (const AccountMessage& message) const {
            if( message.getSenderId() < this->senderId ||
                message.getOriginId() < this->originId ||
                message.getContent() <  this->command){
                return true;
            }
        }
    };
}


#endif //VAA_ACCOUNTMESSAGE_HPP
