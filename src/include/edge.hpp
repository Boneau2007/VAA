
#ifndef EDGE_HPP
#define EDGE_HPP

namespace Uebung1 {

    class Edge{

        private:
            // Classattributes
            unsigned int fromNodeId;
            unsigned int toNodeId;
            unsigned int value;

        public:
            // Constructors
            Edge( unsigned int fromNodeId, unsigned int toNodeId, unsigned int value);

            // Deconstructors
            //~Edge(){}

            // Inline-Elemtfunctions
            unsigned int getValue() const { return value; }
            void setValue(unsigned int value){ this->value = value; }

            unsigned int getFromNodeId() const { return fromNodeId; }
            void setFromNodeId(unsigned int fromNodeId){ this->fromNodeId = fromNodeId; }

            unsigned int getToNodeId() const { return toNodeId; }
            void setToNodeId(unsigned int toNodeId){ this->toNodeId = toNodeId; }


            // Memberfunctions
            std::string toString() const;
    };
}
#endif // EDGE_HPP
