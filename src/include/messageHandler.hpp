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
            explicit MessageHandler(Uebung1::Node* node);
            MessageHandler(Uebung1::Node* node, std::string message);

            // Deconstructors
            
            // Memberfunctions     
            void sendMessage(Uebung1::Message& message, Uebung1::Node& targetNode);
            void handleIncommingMessage();

        private:
            bool wasLastNotVirtualNodeEdge();
            std::vector<Uebung1::Node> getRandNodeIdList(unsigned int maxNumber);
            
            static bool hasElement(std::vector<Uebung1::Node> neighbors, unsigned int id);
            void sendBelieve();
            void startUnificationProcess();
            void sendExplorer(unsigned int senderId);
            void sendEcho(unsigned int senderId, unsigned int virtualId);
            void initRumor();
            void forwardRumor(unsigned int senderId);
    };
}
#endif // MESSAGE_HANDLER_HPP
