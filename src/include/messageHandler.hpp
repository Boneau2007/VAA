#ifndef MESSAGE_HANDLER_HPP
#define MESSAGE_HANDLER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <mutex>
#include "node.hpp"
#include "message.hpp"
#include "echoHandler.hpp"
#include "doubleCounting.hpp"
#include "agreement.hpp"

namespace Uebung1{
    class Node;
    class Message;
    class EchoHandler;
    class DoubleCounting;
    class Agreement;
    class MessageHandler{

        private:
        // Classattributes
            Uebung1::Node* node;
            std::mutex forwardMutex;
            std::mutex echoMutex;
            EchoHandler* echoHandler;
            DoubleCounting* doubleCounting;
            Agreement* agreement;

        public:
            // Constructors
            MessageHandler();
            explicit MessageHandler(Uebung1::Node* node);

            // Deconstructors
        
            DoubleCounting* getDoubleCounting() { return doubleCounting; }
            Agreement* getAgreement() { return agreement; }
        
            // Memberfunctions
            void handleIncommingMessage(Uebung1::Message* msg);

    };
}
#endif // MESSAGE_HANDLER_HPP
