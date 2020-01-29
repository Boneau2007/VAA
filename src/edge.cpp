#include <sstream>
#include <regex>
#include "edge.hpp"

using namespace std;
using namespace Uebung1;

Edge::Edge(unsigned int fromNodeId, unsigned int toNodeId)
    : fromNodeId(fromNodeId), toNodeId(toNodeId), value(0){}

Edge::Edge(unsigned int fromNodeId, unsigned int toNodeId, unsigned int value) 
    : fromNodeId(fromNodeId), toNodeId(toNodeId), value(value){}

void Edge::decode(const string edgeAsString){
    smatch matches;
    const regex isMessageRegex(R"(([0-9]*) -- ([0-9]*);)");
    if(regex_match(edgeAsString, matches, isMessageRegex)){
        fromNodeId = stoi(matches.str(1));
        toNodeId = stoi(matches.str(2));
    }else{
        throw runtime_error("ERROR: No matching line");
    }
}

string Edge::encode(){
    return toString();
}

string Edge::toString() const {
    ostringstream stream;
    stream << fromNodeId << " -- " << toNodeId << ";";
    return stream.str();   
}
