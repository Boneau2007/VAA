//
// Created by alhuber on 31.01.20.
//

#ifndef VAA_ACCOUNT_HPP
#define VAA_ACCOUNT_HPP

#include <vector>
#include <map>
#include "../Graph/node.hpp"

namespace Handler{

    class Node;

    class AccountHandler {

        private:
            Node *node;
            std::map<Message, std::vector<unsigned int>> requestQueue;
            unsigned int balance;
            unsigned int percent;

        public:
            //Contructors
            AccountHandler();

            AccountHandler(Node *node);

            //Inline-Memberfunction
            std::map<Message, std::vector<unsigned int>> getRequestQueue() {return requestQueue;}
            unsigned int getBalanceAmount(){ return balance; }
            unsigned int getPercent(){ return percent; }
            void setPercent(unsigned int percent){ this->percent = percent; }

            //Memberfunctions
            void handleIncommingMessages(Message *message);
            void pushMessageToQueue(const Message& message);
            void removeFromMessageQueue(unsigned int id);
            bool receivedReplyFromAll();
            bool isLowestTimestamp();
            std::vector<unsigned int> getBlockingNodeIdList();
    };
}
#endif //VAA_ACCOUNT_HPP
