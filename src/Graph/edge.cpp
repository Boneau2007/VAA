#include <sstream>
#include <regex>
#include "Include/edge.hpp"

using namespace std;
using namespace Graph;

Edge::Edge(const unsigned int fromNodeId, const unsigned int toNodeId)
    : fromNodeId(fromNodeId), toNodeId(toNodeId), value(0){}

Edge::Edge(const unsigned int fromNodeId, const unsigned int toNodeId, const unsigned int value)
    : fromNodeId(fromNodeId), toNodeId(toNodeId), value(value){}

Edge::Edge(string& edgeAsString) : value(0){
    smatch matches;
    const regex isMessageRegex(R"(([0-9]*) -- ([0-9]*);)");
    if(regex_match(edgeAsString, matches, isMessageRegex)){
        fromNodeId = stoi(matches.str(1));
        toNodeId = stoi(matches.str(2));
    }else{
        throw runtime_error("ERROR: No matching line");
    }
}

string Edge::toString() const {
    ostringstream stream;
    stream << fromNodeId << " -- " << toNodeId << ";";
    return stream.str();   
}
