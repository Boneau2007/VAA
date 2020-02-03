#ifndef I_Message_HPP
#define I_Message_HPP

#include <string>
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
        std::vector<Graph::Edge> hopList;
        time_t time;
        std::vector<unsigned int> orderedBlockedProcessList;    //obpl
        unsigned int balance;
        unsigned int percent;
    public:


        virtual unsigned int getSenderId() const { return senderId; }
        virtual void setSenderId(unsigned int id){ senderId = id; }

        virtual MESSAGE_TYPE getMessageType() const { return messageType; }
        virtual void setMessageType(MESSAGE_TYPE type){ messageType = type; }

        virtual std::string getCommand() const { return command; }
        virtual void setCommand(std::string content){ this->command = content; }

        virtual unsigned int getOriginId() const { return originId; }
        virtual void setOriginId(unsigned int originId){ this->originId = originId; }

        virtual std::vector<Graph::Edge> getHopList() const { return hopList; }
        virtual void setHopList(std::vector<Graph::Edge> hopList){ this->hopList = hopList; }

        virtual time_t getTimestamp() const { return time; }
        virtual void setTimestamp(time_t time){ this->time = time; }

        unsigned int getBalance(){ return balance; }
        void setBalance(unsigned int balance){ this->balance = balance; }

        unsigned int getPercent(){ return percent; }
        void setPercent(unsigned int percent){this->percent = percent; }

        virtual std::vector<unsigned int> getOrderedBlockedProcessList() const { return orderedBlockedProcessList; }
        virtual void setOrderedBlockedProcessList(std::vector<unsigned int> orderedBlockedProcessList){ this->orderedBlockedProcessList = orderedBlockedProcessList; }
    };
}
#endif // I_Message_HPP
