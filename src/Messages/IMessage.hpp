#ifndef I_Message_HPP
#define I_Message_HPP

#include <string>

namespace Messages{
    
    enum MESSAGE_TYPE {APPLICATION, CONTROL};
    
    class IMessage{

    protected:
        // Classattributes
        unsigned int senderId;
        MESSAGE_TYPE messageType;
        std::string command;

    public:
        unsigned int getSenderId() const { return senderId; }
        void setSenderId(unsigned int id){ senderId = id; }

        MESSAGE_TYPE getMessageType() const { return messageType; }
        void setMessageType(MESSAGE_TYPE type){ messageType = type; }

        std::string getCommand() const { return command; }
        void setCommand(std::string content){ this->command = content; }
    };
}
#endif // I_Message_HPP
