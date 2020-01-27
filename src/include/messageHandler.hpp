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
    class MessageHandler{

        private:
            // Classattributes
            Uebung1::Node* node{};
            Uebung1::Message message;
            std::mutex forwardMutex;
            EchoHandler echoHandler;
        
        public:
            // Constructors
            MessageHandler();
            explicit MessageHandler(Uebung1::Node* node);

            // Deconstructors
            
            // Memberfunctions
            void handleIncommingMessage(const Uebung1::Message& msg);
    };
}
#endif // MESSAGE_HANDLER_HPP
