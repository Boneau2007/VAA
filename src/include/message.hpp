#ifndef HTTP_MESSAGE_HPP
#define HTTP_MESSAGE_HPP

#include <iostream>
#include <string>
#include "node.hpp"
#include "edge.hpp"

namespace Uebung1{
    
    enum MESSAGE_TYPE {APPLICATION, CONTROL};
    
    class Message{

        private:
            // Classattributes
            unsigned int senderId; 
            MESSAGE_TYPE type;
            std::string content;
            unsigned int originId;
            std::vector<Edge> hopList;

        public:
            // Constructors
            Message();
            Message(std::string content);
            Message(unsigned int senderId, MESSAGE_TYPE type);
            Message(unsigned int senderId, MESSAGE_TYPE type, std::string content);
            Message(unsigned int senderId, MESSAGE_TYPE type, std::string content, unsigned int originId);
            Message(unsigned int senderId, MESSAGE_TYPE type, std::string content, unsigned int originId,
                    std::vector<Edge> hopList);

            // Deconstructors
            //virtual ~Message();

            //Inline-Elementfunctions
            unsigned int getSenderId() const { return senderId; }
            void setSenderId(unsigned int senderId){ this->senderId = senderId; }

            MESSAGE_TYPE getMessageType() const { return type; }
            void setMessageType(MESSAGE_TYPE type){ this->type = type; }

            std::string getContent() const { return content; }
            void setContent(std::string content){ this->content = content; }

            unsigned int getOriginId() const { return originId; }
            void setOriginId(unsigned int originId){ this->originId = originId; }

            std::vector<Edge> getHopList() const { return hopList; }
            void setHopList(std::vector<Edge> hopList){ this->hopList = hopList; }

            // Memberfunctions
            std::string encode();
            void decode(const std::string& messageAsString);
            std::string toString();
    };
}
#endif // HTTP_MESSAGE_HPP
