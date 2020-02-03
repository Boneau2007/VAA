//
// Created by alhuber on 27.01.20.
//

#ifndef VAA_DOUBLECOUNTING_HPP
#define VAA_DOUBLECOUNTING_HPP

#include "../../Graph/Include/node.hpp"
namespace Application{
    class Node;
    class DoubleCounting {

    private:
        Node* node;
    public:
        DoubleCounting();
        DoubleCounting(Node* node);

    };
}


#endif //VAA_DOUBLECOUNTING_HPP
