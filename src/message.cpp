#include <regex>
#include <sstream>
#include "message.hpp"

using namespace std;
using namespace Uebung1;

Message::Message()
    : senderId(0), type(MESSAGE_TYPE::APPLICATION), content(""){}

Message::Message(const unsigned int senderId, const MESSAGE_TYPE type)
    : senderId(senderId), type(type), content(""){}

Message::Message(const unsigned int senderId, const MESSAGE_TYPE type, const std::string content)
    : senderId(senderId), type(type), content(content){}

Message::Message(const std::string message){
    smatch matches;
    const regex isMessageRegex(R"(([0-9]*);(app|ctrl);(.*))");
    if(regex_match(message, matches, isMessageRegex)){
        senderId = stoi(matches.str(1));
        matches.str(2).compare("app")==0 ? type=APPLICATION : type=CONTROL;
        content = matches.str(3);
    }else{
        throw runtime_error("ERROR: No matching line");
    }
}

string Message::toString(){
    ostringstream ss;
    ss  << senderId;
    type == APPLICATION ? ss << ";app;" : ss << ";ctrl;" ;
    ss << content;
    return ss.str();
}
