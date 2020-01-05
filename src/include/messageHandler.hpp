#ifndef MESSAGE_HANDLER_HPP
#define MESSAGE_HANDLER_HPP

#include <iostream>
#include <string>
#include <vector>
#include "node.hpp"
#include "message.hpp"

namespace Uebung1{
    class Node;
    class MessageHandler{

        private:
            // Classattributes
            Uebung1::Node node;
            Uebung1::Message message;
            std::vector< unsigned int> believers;
            std::vector<unsigned int> activeNodeList;
            unsigned int recvRumors;
            bool sendedRumor;
        
        public:
            // Constructors
            MessageHandler();
            MessageHandler(Uebung1::Node& node);
            MessageHandler(Uebung1::Node& node, std::string message);
            // Deconstructors
            //virtual ~Message();

            //Inline-Elementfunctions
            bool getSendedRumor(){return sendedRumor;}
            void setSendedRumor(bool sendedRumor){this->sendedRumor = sendedRumor;}

            std::vector<unsigned int> getActiveNodeList(){ return activeNodeList;}
            void getActiveNodeList(std::vector<unsigned int> activeNodeList){ this->activeNodeList=activeNodeList;}
 
            // Memberfunctions     
            void sendMessage(Uebung1::Message& message, Uebung1::Node& targetNode);
            void handleIncommingMessage();
            void sendPing();

        private:
            void sendBelieve();
            void initRumor();
            void forwardRumor();
            void setHostAddress(const Uebung1::Node& node);
    };
}
#endif // MESSAGE_HANDLER_HPP
