//
// Created by alhuber on 02.02.20.
//

#ifndef VAA_MESSAGEIMPL_HPP
#define VAA_MESSAGEIMPL_HPP

#include <string>
#include <regex>
#include <sstream>
#include "IMessage.hpp"

namespace Messages{

    class Message : public IMessage {

    public:
        Message() {
            this->senderId = 0;
            this->messageType = MESSAGE_TYPE::APPLICATION;
            this->command = "";
            originId = 0;
            recvLocalClock = 0;
            localClock = 0;
            balance = 0;
            percent = 0;
        }

        Message(const unsigned int senderId, MESSAGE_TYPE messageType) : Message() {
            this->senderId = senderId;
            this->messageType = messageType;
        }

        Message(const unsigned int senderId, const MESSAGE_TYPE type, std::string command) : Message(senderId, type) {
            this->command = std::move(command);
        }


        explicit Message(const std::string& content) : Message() {
            std::smatch matches;
            const std::regex isMessageRegex(R"(([0-9]+);(app|ctrl);([a-zA-Z: 0-9]+);)");
            if(regex_match(content, matches, isMessageRegex)){
                senderId = stoi(matches.str(1));
                matches.str(2)=="app" ? messageType = APPLICATION : messageType = CONTROL;
                command = matches.str(3);
            }else{
                throw std::runtime_error("ERROR: No matching line for Message");
            }
        }


        virtual std::string toString(){
            std::ostringstream ss;
            ss << senderId;
            messageType == APPLICATION ? ss << ";app;" : ss << ";ctrl;" ;
            ss << command << ";";
            return ss.str();
        }

    };

    class LockMessage : public Message {
    public:

        LockMessage() = default;
        LockMessage(const unsigned int senderId, const MESSAGE_TYPE messageType, const std::string& command,
                    const unsigned int lockId, const unsigned int originId, unsigned int localClock)
                : Message(senderId, messageType, command){
            this->lockId = lockId;
            this->originId = originId;
            this->recvLocalClock = recvLocalClock;
        }

        explicit LockMessage(const std::string& content) : Message(){
            std::smatch matches;
            //senderId;type;content;originId;Time
            const std::regex isMessageRegex(R"(([0-9]+);(app|ctrl);([a-zA-Z ]+);([0-9]+);([0-9]+);([0-9]+);)");
            if(regex_match(content, matches, isMessageRegex)){
                senderId = stoi(matches.str(1));
                matches.str(2)=="app" ? messageType=APPLICATION : messageType=CONTROL;
                command = matches.str(3);
                lockId = stoi(matches.str(4));
                originId = stoi(matches.str(5));
                recvLocalClock = stoi(matches.str(6));
            }else{
                throw std::runtime_error("ERROR: No matching line for forwardMessage");
            }
        }

        std::string toString() override {
            std::ostringstream ss;
            ss << senderId;
            messageType == APPLICATION ? ss << ";app;" : ss << ";ctrl;" ;
            ss << command << ";" << lockId << ";" << originId << ";" << recvLocalClock << ";";
            return ss.str();
        }

    };

    class OrderedBlockedMessage : public Message {
    public:
        OrderedBlockedMessage() = default;

        OrderedBlockedMessage(const unsigned int senderId, const MESSAGE_TYPE messageType, const std::string& command)
            : Message(senderId, messageType, command) {}

        explicit OrderedBlockedMessage(const std::string& orderedBlockedMessageString) {
            std::smatch matches;
            const std::regex isMessageRegex(R"(([0-9]+);(app|ctrl);([a-zA-Z ]+);(.*))");
            if(regex_match(orderedBlockedMessageString, matches, isMessageRegex)){
                senderId = stoi(matches.str(1));
                matches.str(2)=="app" ? messageType=APPLICATION : messageType=CONTROL;
                command = matches.str(3);
                std::stringstream obplAsString(matches.str(4));
                std::string obpId;
                while(getline(obplAsString, obpId, ';')){
                    const std::regex isIdRegex(R"(([0-9]+))");
                    std::smatch idMatch;
                    if(regex_match(obpId, idMatch, isIdRegex)){
                        orderedBlockedProcessList.emplace_back(stoi(idMatch.str(1)));
                    }else{
                        throw std::runtime_error("ERROR: No matching line for ordered blocked Process Message: " + orderedBlockedMessageString);
                    }
                }
            }else{
                throw std::runtime_error("ERROR: No matching line is ordered blocked Process Message; " + orderedBlockedMessageString);
            }
        }

        void addObpId(unsigned int id) {
            orderedBlockedProcessList.emplace_back(id);
        }
        std::string toString() override{
            std::ostringstream ss;
            ss << senderId;
            messageType == APPLICATION ? ss << ";app;" : ss << ";ctrl;" ;
            ss << command << ";";
            for(int id : orderedBlockedProcessList){
                ss << id << ";";
            }
            return ss.str();
        }

    };

    class AccountMessage : public Message{
    public:
        AccountMessage() : Message() {}
        AccountMessage( unsigned int senderId, MESSAGE_TYPE type, const std::string& command,
                                        unsigned int balance, unsigned int percent) : Message(senderId, type, command){
            this->balance = balance;
            this->percent = percent;
        }

        explicit AccountMessage(const std::string& accountMsgAsString) : Message(){
            std::smatch matches;
            const std::regex isMessageRegex(R"(([0-9]+);(app|ctrl);([a-zA-Z ]+);([0-9]+);([0-9]+);)");
            if(regex_match(accountMsgAsString, matches, isMessageRegex)){
                senderId = stoi(matches.str(1));
                matches.str(2)=="app" ? messageType=APPLICATION : messageType=CONTROL;
                command = matches.str(3);
                balance = stoi(matches.str(4));
                percent = stoi(matches.str(5));
            }else{
                throw std::runtime_error("ERROR: No matching line");
            }
        }

        std::string toString() override {
            std::ostringstream ss;
            ss << senderId;
            messageType == APPLICATION ? ss << ";app;" : ss << ";ctrl;" ;
            ss << command << ";" << balance <<";" << percent << ";";
            return ss.str();
        }
    };

    class LockAckMessage : public Message {
    public:
        LockAckMessage() : Message() {}
        LockAckMessage( unsigned int senderId, MESSAGE_TYPE type, const std::string& command, unsigned int lockId,
                        unsigned int id, unsigned int localClock, unsigned int originId,  unsigned int recvLocalClock)
                        : Message(senderId, type, command){
        }

        explicit LockAckMessage(const std::string& accountMsgAsString) : Message(){
            std::smatch matches;
            const std::regex isMessageRegex(R"(([0-9]+);(app|ctrl);([a-zA-Z ]+);([0-9]+);([0-9]+);([0-9]+);([0-9]+);([0-9]+);)");
            if(regex_match(accountMsgAsString, matches, isMessageRegex)){
                senderId = stoi(matches.str(1));
                matches.str(2)=="app" ? messageType=APPLICATION : messageType=CONTROL;
                command = matches.str(3);
                lockId = stoi(matches.str(4));
                senderId = stoi(matches.str(5));
                localClock = stoi(matches.str(6));
                originId = stoi(matches.str(7));
                recvLocalClock = stoi(matches.str(8));
            }else{
                throw std::runtime_error("ERROR: No matching line Lock Messages");
            }
        }

        std::string toString() override {
            std::ostringstream ss;
            ss << senderId;
            messageType == APPLICATION ? ss << ";app;" : ss << ";ctrl;" ;
            ss << command << ";" << lockId <<";" << senderId << ";";
            ss << localClock << ";"<< originId << ";"<< recvLocalClock << ";";
            return ss.str();
        }
    };

}
#endif //VAA_MESSAGEIMPL_HPP
