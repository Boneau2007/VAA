#ifndef I_Message_HPP
#define I_Message_HPP

#include <string>
#include <utility>
#include <vector>
#include <map>
#include <ctime>
#include "edge.hpp"

namespace Messages{
    
    enum MESSAGE_TYPE {APPLICATION, CONTROL};
    
    class IMessage{

    protected:
        // Classattributes
        unsigned int senderId;
        MESSAGE_TYPE messageType;
        std::string command;
        unsigned int originId;
        unsigned int lockId;
        std::vector<Graph::Edge> hopList;
        unsigned int localClock;
        unsigned int recvLocalClock;
        std::vector<unsigned int> orderedBlockedProcessList;    //obpl
        unsigned int balance;
        unsigned int percent;
    public:


        virtual unsigned int getSenderId() const { return senderId; }
        virtual MESSAGE_TYPE getMessageType() const { return messageType; }
        virtual std::string getCommand() const { return command; }
        virtual unsigned int getOriginId() const { return originId; }
        virtual unsigned int getLockId() const { return lockId; }
        virtual std::vector<Graph::Edge> getHopList() const { return hopList; }
        virtual unsigned int getRecvLocalClock() const { return recvLocalClock; }
        virtual unsigned int getLocalClock() const { return localClock; }
        virtual unsigned int getBalance(){ return balance; }
        virtual unsigned int getPercent(){ return percent; }
        virtual std::vector<unsigned int> getOrderedBlockedProcessList() const { return orderedBlockedProcessList; }

        virtual void setSenderId(unsigned int senderId){ this->senderId = senderId; }
        virtual void setMessageType(MESSAGE_TYPE messageType){ this->messageType = messageType; }
        virtual void setCommand(std::string command){ this->command = command; }
        virtual void setLockId(unsigned int lockId){ this->lockId = lockId; }
        virtual void setOriginId(unsigned int originId){ this->originId = originId; }
        virtual void setHopList(std::vector<Graph::Edge> hopList){ this->hopList = hopList; }
        virtual void setRecvLocalClock(unsigned int recvLocalClock){ this->recvLocalClock = recvLocalClock; }
        virtual void setLocalClock(unsigned int localClock){ this->localClock = localClock; }
        virtual void setBalance(unsigned int balance){ this->balance = balance; }
        virtual void setPercent(unsigned int percent){this->percent = percent; }
        virtual void setOrderedBlockedProcessList(std::vector<unsigned int> orderedBlockedProcessList){ this->orderedBlockedProcessList = orderedBlockedProcessList; }

        //Operator
        bool operator< (const IMessage& message) const {
            if(message.getRecvLocalClock() < this->recvLocalClock){
                return true;
            }
        }
    };
}
#endif // I_Message_HPP
