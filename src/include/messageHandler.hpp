#ifndef MESSAGE_HANDLER_HPP
#define MESSAGE_HANDLER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <mutex>
#include "node.hpp"
#include "message.hpp"
#include "echoHandler.hpp"

namespace Uebung1{
    class Node;
    class Message;
    class EchoHandler;
    class MessageHandler{

        private:
        // Classattributes
            Uebung1::Node* node;
            std::mutex forwardMutex;
            std::mutex echoMutex;
            EchoHandler* echoHandler;
        public:
            // Constructors
            MessageHandler();
            explicit MessageHandler(Uebung1::Node* node);

            // Deconstructors
            
            // Memberfunctions
            void handleIncommingMessage(Uebung1::Message* msg);

    };
}
#endif // MESSAGE_HANDLER_HPP
