#include <regex>
#include <istream>
#include <utility>
#include "message.hpp"

using namespace std;
using namespace Messages;

Message::Message() {
    setSenderId(0);
    setMessageType(MESSAGE_TYPE::APPLICATION);
    setCommand("");
}

Message::Message(const unsigned int senderId, MESSAGE_TYPE type) {
    setSenderId(senderId);
    setMessageType(type);
    setCommand("");
}

Message::Message(const unsigned int senderId, const MESSAGE_TYPE type, string content) : Message(senderId, type) {
    setCommand(std::move(content));
}


Message::Message(const string& content){
    smatch matches;
    const regex isMessageRegex(R"((([0-9]+);(app|ctrl);(.*);)");
    if(regex_match(content, matches, isMessageRegex)){
        senderId = stoi(matches.str(1));
        matches.str(2)=="app" ? setMessageType(APPLICATION) : setMessageType(CONTROL);
        command = matches.str(3);
    }else{
        throw runtime_error("ERROR: No matching line for Message");
    }
}


string Message::toString(){
    ostringstream ss;
    ss << senderId;
    messageType == APPLICATION ? ss << ";app;" : ss << ";ctrl;" ;
    ss << command << ";";
    return ss.str();
}


//Message::Message(const string& content){
//    //std::vector<Edge> hopedNodeIdList;
//    smatch matches;
//    const regex isMessageRegex(R"(([0-9]+);([0-9]+);([0-9]+);(app|ctrl);(.*) ;(.*))");
//    if(regex_match(content, matches, isMessageRegex)){
//        time = stoi(matches.str(1));
//        originId = stoi(matches.str(2));
//        senderId = stoi(matches.str(3));
//        matches.str(4)=="app" ? type=APPLICATION : type=CONTROL;
//        command = matches.str(5);
//        stringstream hops(matches.str(6));
//        string hop;
//        while(getline(hops, hop, ';')){
//            const regex isHopRegex(R"(([0-9]+) -- ([0-9]+))");
//            smatch hopMatch;
//            if(regex_match(hop, hopMatch, isHopRegex)){
//                hopList.emplace_back(stoi(hopMatch.str(1)), stoi(hopMatch.str(2)));
//            }else{
//                throw runtime_error("ERROR: No matching line hops");
//            }
//
//        }
//    }else{
//        throw runtime_error("ERROR: No matching line");
//    }
//}