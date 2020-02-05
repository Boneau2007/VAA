//
// Created by alhuber on 31.01.20.
//

#ifndef VAA_ACCOUNT_HPP
#define VAA_ACCOUNT_HPP

#include <vector>
#include <map>
#include "node.hpp"
#include "messages.hpp"

namespace Graph{
    class Node;
}

namespace Handler{
    class AccountHandler {

        private:
            Graph::Node* node;
            unsigned int balance;
            unsigned int percent;

        public:
            //Contructors
            AccountHandler();

            AccountHandler(Graph::Node *node);

            //Inline-Memberfunction
            unsigned int getBalanceAmount(){ return balance; }
            unsigned int getPercent(){ return percent; }
            void setPercent(unsigned int percent){ this->percent = percent; }

            //Memberfunctions
            void handleIncommingMessage(const std::string& msg, const time_t time);
    };
}
#endif //VAA_ACCOUNT_HPP
