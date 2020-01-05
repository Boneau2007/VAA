#ifndef HTTP_MESSAGE_HPP
#define HTTP_MESSAGE_HPP

#include <iostream>
#include <string>

namespace Uebung1{
    
    enum MESSAGE_TYPE {APPLICATION, CONTROL};
    
    class Message{

        private:
            // Classattributes
            unsigned int senderId; 
            MESSAGE_TYPE type;
            std::string content;
        
        public:
            // Constructors
            Message();
            Message(const unsigned int senderId, const MESSAGE_TYPE type);
            Message(const unsigned int senderId, const MESSAGE_TYPE type, const std::string content);
            Message(const MESSAGE_TYPE type, const std::string content);
            Message(const std::string message);
            
            // Deconstructors
            //virtual ~Message();

            //Inline-Elementfunctions
            unsigned int getSenderId() const { return senderId; }
            void setSenderId(unsigned int senderId){ this->senderId = senderId; }

            MESSAGE_TYPE getMessageType() const { return type; }
            void setMessageType(MESSAGE_TYPE type){ this->type = type; }

            std::string getContent() const { return content; }
            void setContent(std::string content){ this->content = content; }

            // Memberfunctions
            std::string toString();
    };
}
#endif // HTTP_MESSAGE_HPP
