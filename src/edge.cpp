#include <sstream>
#include "edge.hpp"

using namespace std;
using namespace Uebung1;

Edge::Edge(unsigned int fromNodeId, unsigned int toNodeId, unsigned int value) 
    : fromNodeId(fromNodeId), toNodeId(toNodeId), value(value){}

string Edge::toString() const{
    ostringstream stream;
    stream << fromNodeId << " -- " << toNodeId << ";" << endl;
    return stream.str();   
}