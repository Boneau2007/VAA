//
// Created by alhuber on 04.02.20.
//

#ifndef VAA_FLOODHANDLER_HPP
#define VAA_FLOODHANDLER_HPP

#include <string>
#include <ctime>
#include "node.hpp"
#include "messages.hpp"
namespace Graph{
    class Node;
}

namespace Handler{
    class LockHandler {
    private:
        Graph::Node* node;
    public:
        LockHandler();
        LockHandler(Graph::Node* node);
        void handleIncommingMessage(const std::string& msg, const time_t time);

    };
}

#endif //VAA_FLOODHANDLER_HPP
