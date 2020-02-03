//
// Created by alhuber on 02.02.20.
//

#include <sstream>
#include <regex>
#include "Include/forwardMessage.hpp"

using  namespace Messages;
using namespace std;

ForwardMessage::ForwardMessage() {

}

ForwardMessage::ForwardMessage(unsigned int senderId, MESSAGE_TYPE type) : Message(senderId, type) {

}

ForwardMessage::ForwardMessage(unsigned int senderId, MESSAGE_TYPE type, string content)
        : Message(senderId, type, content) {

}

ForwardMessage::ForwardMessage(const string& content){
    smatch matches;
    //originId;senderId;type;content;
    const regex isMessageRegex(R"(([0-9]+);([0-9]+);(app|ctrl);(.*);)");
    if(regex_match(content, matches, isMessageRegex)){
        originId = stoi(matches.str(1));
        senderId = stoi(matches.str(2));
        matches.str(3)=="app" ? messageType=APPLICATION : messageType=CONTROL;
        command = matches.str(4);
    }else{
        throw runtime_error("ERROR: No matching line for forwardMessage");
    }
}

string ForwardMessage::toString() {
    ostringstream ss;
    ss << originId << ";" << senderId;
    messageType == APPLICATION ? ss << ";app;" : ss << ";ctrl;" ;
    ss << command << ";";
    return ss.str();
}
