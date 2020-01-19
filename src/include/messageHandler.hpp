#ifndef MESSAGE_HANDLER_HPP
#define MESSAGE_HANDLER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <mutex>
#include "node.hpp"
#include "message.hpp"

namespace Uebung1{
    class MessageHandler{

        private:
            // Classattributes
            Uebung1::Node* node;
            Uebung1::Message message;
            std::mutex forwardMutex;
            unsigned int recvRumors;
        
        public:
            // Constructors
            MessageHandler();
            MessageHandler(Uebung1::Node* node);
            MessageHandler(Uebung1::Node* node, std::string message);

            // Deconstructors
            
            // Memberfunctions     
            void sendMessage(Uebung1::Message& message, Uebung1::Node& targetNode);
            void handleIncommingMessage();

        private:
            void sendBelieve();
            void sendExplorer(const unsigned int senderId);
            void sendEcho();
            void initRumor();
            void forwardRumor(const unsigned int senderId);
            void setHostAddress(const Uebung1::Node& node);
    };
}
#endif // MESSAGE_HANDLER_HPP
