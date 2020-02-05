//
// Created by alhuber on 02.02.20.
//

#ifndef VAA_GOLDMANEDGECHASINGHANDLER_HPP
#define VAA_GOLDMANEDGECHASINGHANDLER_HPP

#include <ctime>
#include <string>
#include <vector>
#include "node.hpp"
#include "messages.hpp"
namespace Graph{
    class Node;
}
namespace Handler{
    class GoldmanEdgeChasingHandler{
    private:
        Graph::Node* node;

    public:
        //Contructors
        GoldmanEdgeChasingHandler();
        explicit GoldmanEdgeChasingHandler(Graph::Node *node);

        //Memberfunctions
        void handleIncommingMessages(const std::string& message, time_t time);
    };

}


#endif //VAA_GOLDMANEDGECHASINGHANDLER_HPP
