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
            std::map<Messages::AccountMessage, std::vector<unsigned int>> requestQueue;
            unsigned int balance;
            unsigned int percent;

        public:
            //Contructors
            AccountHandler();

            AccountHandler(Graph::Node *node);

            //Inline-Memberfunction
            std::map<Messages::AccountMessage, std::vector<unsigned int>> getRequestQueue() {return requestQueue;}
            unsigned int getBalanceAmount(){ return balance; }
            unsigned int getPercent(){ return percent; }
            void setPercent(unsigned int percent){ this->percent = percent; }

            //Memberfunctions
            void handleIncommingMessage(const std::string& msg, const time_t time);
            void pushMessageToQueue(const Messages::AccountMessage& message);
            std::vector<unsigned int> getBlockingNodeIdList();
    };
}
#endif //VAA_ACCOUNT_HPP
