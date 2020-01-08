#include <fstream>  // ifstream()
#include <regex>    // regex()
#include "fileHandler.hpp"

using namespace std;
using namespace Uebung1;

FileHandler::FileHandler()
    : nodeFile(""), graphFile("") {}

FileHandler::FileHandler(const string nodeFile) 
    : nodeFile(nodeFile), graphFile(""){
        nodeList = readNodes();
        // for(unsigned int i=0;i < nodeList.size();i++){
        //     cout << nodeList.at(i).toString() << endl;
        // }
    }

FileHandler::FileHandler(const string nodeFile, const string graphFile) 
    : nodeFile(nodeFile), graphFile(graphFile){
        nodeList = readNodes();
    }

vector<Node> FileHandler::readNodes(){
    vector<Node> nodeList;
    string line;
    smatch matches;
    ifstream fileStream;
    const regex isNodeLineRegex(R"(([0-9]*) (.*):([0-9]*))");    
    if(nodeFile.empty()){
        throw  runtime_error("ERROR: file name empty");
    }
    fileStream.open(nodeFile, ios::out);
    if(!fileStream.is_open()){
        throw  runtime_error("Can't open file");
    }else{
            for(unsigned int i=0; getline(fileStream, line); i++){
                if(line.empty()){
                    //cout << "Skip empty line" << endl;
                    continue;
                }else if(regex_match(line, matches, isNodeLineRegex)){
                    //cout << matches.str(1) << " " << matches.str(2)  << " " << matches.str(3) << endl;
                    Node node(stoi(matches.str(1)), matches.str(2), stoi(matches.str(3)));
                    nodeList.push_back(node);
                }else{
                    throw runtime_error("ERROR: No matching line");
                }
            }
    }
    fileStream.close();
    return nodeList;
}

vector<Node> FileHandler::readGraphviz(const unsigned int id){
    ifstream fileStream;
    string::size_type sz;
    string line;
    smatch matches;
    vector<Node> nodeList;
    const regex nameLine(R"(graph ([a-zA-z]+) \{)");
    const regex graphvizNodeLine(R"(([0-9]+) -- ([0-9]+);)");
    const regex endLine(R"(})");
    if(graphFile.empty()){
        throw  runtime_error("ERROR: file name empty");
    }
    fileStream.open(graphFile, ios::out);
    if(!fileStream.is_open()){
        throw  runtime_error("Can't open file");
    }else{
        if(fileStream.peek() == ifstream::traits_type::eof()){
                throw runtime_error("ERROR: Empty file ");
        }
        while(getline(fileStream, line)){
            if(regex_match(line, matches, nameLine)){
                continue;
            }else if(regex_match(line, matches, graphvizNodeLine)){
                unsigned int fromId = stoi(matches.str(1), &sz);
                unsigned int toId = stoi(matches.str(2), &sz);
                if(fromId == id){
                    nodeList.push_back(getNodeFromFile(toId));
                }else if(toId == id){
                    nodeList.push_back(getNodeFromFile(fromId));
                }
            }else if(regex_match(line, matches, endLine)){
                continue;
            }else{
                fileStream.close();
                throw runtime_error("ERROR: No matching regex");
            }
        }
        fileStream.close();
        return nodeList;
    }
}

Node FileHandler::getNodeFromFile(const unsigned int id){
    for(unsigned int i=0;i < nodeList.size();i++){
        if(nodeList.at(i).getId() == id){
            return nodeList.at(i);
        }
    }
    throw runtime_error("Node not found");
}

void FileHandler::graphgen(const string fileName, const unsigned int edgeCount){
    vector<Edge> edgeList;
    srand(time(0));
    //Ausnahme bei N < 4
    if(edgeCount <= nodeList.size() || edgeCount > (nodeList.size()*nodeList.size()-nodeList.size())/2){
        throw runtime_error("ERROR: Num of edges to large or to small");
    }
        for(unsigned int i=0; i < nodeList.size(); i++){
            while(true){
                unsigned int randId = rand()%nodeList.size();
                if(nodeList.at(i).getId() != nodeList.at(randId).getId() && !containsEdge(edgeList,nodeList.at(i).getId(), nodeList.at(randId).getId())){
                Edge edge(nodeList.at(i).getId(), nodeList.at(randId).getId(),1);
                edgeList.push_back(edge);
                break;
                }
            }
        }
    cout << nodeList.size()<<  " " << edgeCount << " " << edgeList.size()<< endl;
    writeGraphvizFile(fileName, edgeList);
}

bool FileHandler::containsEdge(const vector<Edge> edgeList, const unsigned int idFrom, const unsigned int idTo){
    for(unsigned int i=0;i < edgeList.size(); i++){
            if(edgeList.at(i).getFromNodeId() == idFrom && 
               edgeList.at(i).getToNodeId() == idTo){
                   return true;
            }else if(edgeList.at(i).getToNodeId() == idFrom && 
               edgeList.at(i).getFromNodeId() == idTo) {
                   return true;
            }
    }
    return false;
}

void FileHandler::writeGraphvizFile(const string fileName, const vector<Edge> edgeList){
ostringstream stream;
    if(fileName.empty()){
        throw  runtime_error("ERROR: file name empty");
    }
    ofstream fileStream(fileName, ios::trunc);
    if(!fileStream.is_open()){
        throw  runtime_error("Can't open file");
    }else{
        stream << "graph G {" << endl; 
        for(unsigned int i=0; i < edgeList.size(); i++){
            stream << edgeList.at(i).toString();
        }
        stream << "}";
        fileStream.write(stream.str().c_str(), stream.str().size());
        fileStream.close();
    }
}
