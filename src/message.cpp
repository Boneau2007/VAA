#include <regex>
#include <sstream>
#include <istream>
#include <utility>
#include "message.hpp"

using namespace std;
using namespace Uebung1;

Message::Message()
    : senderId(0), type(MESSAGE_TYPE::APPLICATION), content(""), originId(0){}

Message::Message(unsigned int senderId, MESSAGE_TYPE type)
        : senderId(senderId), type(type), originId(senderId) {}

Message::Message(const unsigned int senderId, const MESSAGE_TYPE type, const string content)
        : senderId(senderId), type(type), content(content), originId(senderId){}

Message::Message(const unsigned int senderId, const MESSAGE_TYPE type, const string content, const unsigned int originId)
        : senderId(senderId), type(type), content(content), originId(originId){}

Message::Message(const unsigned int senderId, const MESSAGE_TYPE type, const string content, const unsigned int originId,
                 const vector<Edge> hopList)
                 : senderId(senderId), type(type), content(content), originId(originId), hopList(hopList){}

Message::Message(std::string content) {
    decode(std::move(content));
}

void Message::decode(const string& messageAsString){
    //std::vector<Edge> hopedNodeIdList;
    smatch matches;
    const regex isMessageRegex(R"(([0-9]+);([0-9]+);(app|ctrl);(.*) ;(.*))");
    if(regex_match(messageAsString, matches, isMessageRegex)){
        originId = stoi(matches.str(1));
        senderId = stoi(matches.str(2));
        matches.str(3)=="app" ? type=APPLICATION : type=CONTROL;
        content = matches.str(4);
        stringstream hops(matches.str(5));
        string hop;
        while(getline(hops, hop, ';')){
            const regex isHopRegex(R"(([0-9]+) -- ([0-9]+))");
            smatch hopMatch;
            if(regex_match(hop, hopMatch, isHopRegex)){
                hopList.push_back(Edge(stoi(hopMatch.str(1)), stoi(hopMatch.str(2))));
            }else{
                throw runtime_error("ERROR: No matching line hops");
            }

        }
    }else{
        throw runtime_error("ERROR: No matching line");
    }
}

string Message::encode(){
    return toString();
}

string Message::toString(){
    ostringstream ss;
    ss << originId << ";" << senderId;
    type == APPLICATION ? ss << ";app;" : ss << ";ctrl;" ;
    ss << content << " ;";
    for(auto& i : hopList){
        ss << i.toString();
    }
    return ss.str();
}

