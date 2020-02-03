#ifndef MESSAGE_HANDLER_HPP
#define MESSAGE_HANDLER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <mutex>
#include "../Graph/node.hpp"
#include "../Messages/IMessage.hpp"
#include "echoHandler.hpp"
#include "../Application/doubleCounting.hpp"
#include "../Application/agreement.hpp"
#include "accountHandler.hpp"
#include "goldmanEdgeChasingHandler.hpp"

namespace Handler{
    class Node;
    class Message;
    class EchoHandler;
    class DoubleCounting;
    class Agreement;
    class AccountHandler;
    //class GoldmanEdgeChasingHandler;
    class MessageHandler{

        private:
        // Classattributes
            Node* node;
            std::mutex forwardMutex;
            std::mutex echoMutex;
            std::mutex accountMutex;
            std::mutex goldmanMutex;
            EchoHandler* echoHandler;
            AccountHandler* accountHandler;
            DoubleCounting* doubleCounting;
            GoldmanEdgeChasingHandler* goldmanEdgeChasingHandler;
            Agreement* agreement;

        public:
            // Constructors
            MessageHandler();
            explicit MessageHandler(Node* node);

            // Deconstructors
        
            DoubleCounting* getDoubleCounting() { return doubleCounting; }
            Agreement* getAgreement() { return agreement; }

            AccountHandler* getAccountHandler(){ return accountHandler; }
            GoldmanEdgeChasingHandler* getGoldmanEdgeChasingHandler(){ return goldmanEdgeChasingHandler; }
        
            // Memberfunctions
            void handleIncommingMessage(Message* msg);
    };
}
#endif // MESSAGE_HANDLER_HPP
