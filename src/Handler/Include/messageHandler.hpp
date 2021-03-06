#ifndef MESSAGE_HANDLER_HPP
#define MESSAGE_HANDLER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <mutex>
#include "node.hpp"
#include "IMessage.hpp"
#include "echoHandler.hpp"
#include "doubleCounting.hpp"
#include "agreement.hpp"
#include "accountHandler.hpp"
#include "goldmanEdgeChasingHandler.hpp"
#include "floodHandler.hpp"
namespace Graph{
    class Node;
}

namespace Messages{
    class IMessage;
}


namespace Application{
    class DoubleCounting;
    class Agreement;
}

namespace Handler{
    class EchoHandler;
    class AccountHandler;
    class GoldmanEdgeChasingHandler;
    class LockHandler;
    class MessageHandler{

    private:// Classattributes
        Graph::Node* node;
        std::mutex forwardMutex;
        std::mutex echoMutex;
        std::mutex accountMutex;
        std::mutex lockMutex;
        std::mutex goldmanMutex;
        Handler::EchoHandler* echoHandler;
        Handler::AccountHandler* accountHandler;
        Handler::GoldmanEdgeChasingHandler* goldmanEdgeChasingHandler;
        Handler::LockHandler* lockHandler;
        Application::DoubleCounting* doubleCounting;
        Application::Agreement* agreement;

    public:
        // Constructors
        MessageHandler();
        explicit MessageHandler(Graph::Node* node);

        // Deconstructors
        Application::DoubleCounting* getDoubleCounting() { return doubleCounting; }
        Application::Agreement* getAgreement() { return agreement; }

        Handler::AccountHandler* getAccountHandler(){ return accountHandler; }
        Handler::GoldmanEdgeChasingHandler* getGoldmanEdgeChasingHandler(){ return goldmanEdgeChasingHandler; }

        // Memberfunctions
        void handleIncommingMessage(const std::string& msg, const time_t time);
    };
}
#endif // MESSAGE_HANDLER_HPP
