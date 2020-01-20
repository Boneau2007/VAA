#include <fstream>  // ifstream()
#include <regex>    // regex()
#include "fileHandler.hpp"

using namespace std;
using namespace Uebung1;

FileHandler::FileHandler()
    : nodeFile(""), graphFile("") {}

FileHandler::FileHandler(const string nodeFile) 
    : nodeFile(nodeFile), graphFile(""){
}

FileHandler::FileHandler(const string nodeFile, const string graphFile) 
    : nodeFile(nodeFile), graphFile(graphFile){
}

/*
 * This function parses a node file in a List of Nodes
 */
vector<Node> FileHandler::readNodes(const unsigned int maxNodesToRead){
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
        // Are there more lines in file and is the requested nodeListSize reached 
        for(unsigned int i=0; getline(fileStream, line); i++){
            if(line.empty()){
                continue;
            }else if(nodeList.size() == maxNodesToRead){
                break;
            }else if(regex_match(line, matches, isNodeLineRegex)){
                //cout << matches.str(1) << " " << matches.str(2)  << " " << matches.str(3) << endl;
                Node node(stoi(matches.str(1)), matches.str(2), stoi(matches.str(3)));
                nodeList.push_back(node);
            }else{
                throw runtime_error("ERROR: No matching line");
            }
        }
        if(nodeList.size() < maxNodesToRead){
            throw runtime_error("ERROR: Less nodes in File than required");
        }
    }
    fileStream.close();
    return nodeList;
}


/*
 * This function parses a graphviz file and sets the neighbors to the given Id
 */
vector<Node> FileHandler::readGraphviz(const unsigned int id, vector<Node> nodeList){
    ifstream fileStream;
    string::size_type sz;
    string line;
    smatch matches;
    vector<Node> neighborList;
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
                    neighborList.push_back(getNodeFromFile(toId, nodeList));
                }else if(toId == id){
                    neighborList.push_back(getNodeFromFile(fromId, nodeList));
                }
            }else if(regex_match(line, matches, endLine)){
                continue;
            }else{
                fileStream.close();
                throw runtime_error("ERROR: No matching regex");
            }
        }
        fileStream.close();
        return neighborList;
    }
}

/*
 * This function helps to find a Node in the nodeList with a given id
 */
Node FileHandler::getNodeFromFile( const unsigned int id, vector<Node> nodeList){
    for(unsigned int i=0;i < nodeList.size();i++){
        if(nodeList.at(i).getId() == id){
            return nodeList.at(i);
        }
    }
    throw runtime_error("Node not found");
}


/*
 * This function generates a graphviz file with a given name, the nodeList and the number of edges
 */
void FileHandler::graphgen(const string fileName, vector<Node> nodeList, const unsigned int nodesNumber, const unsigned int edgeCount){
    vector<Edge> edgeList;
    srand(time(0));
    //Ausnahme bei N < 4
    if(edgeCount <= nodesNumber || edgeCount > (nodesNumber*nodesNumber-nodesNumber)/2){
        throw runtime_error("ERROR: Num of edges to large or to small");
    }
    while(edgeList.size() < edgeCount){
        for(unsigned int i=0; i < nodesNumber; i++){
        // cout << nodesNumber<<  " " << edgeCount << " " << edgeList.size()<< endl;
            while(edgeList.size() < edgeCount){
                unsigned int randId = rand()%nodesNumber;
                if(nodeList.at(i).getId() != nodeList.at(randId).getId() && !containsEdge(edgeList, nodeList.at(i).getId(), nodeList.at(randId).getId())){
                    Edge edge(nodeList.at(i).getId(), nodeList.at(randId).getId(),1);
                    edgeList.push_back(edge);
                    break;
                }
                if(maxEdgesPerNode(edgeList, nodeList.at(i).getId(), nodesNumber-1)){
                    break;
                }
            }
        }
    }
    // cout << nodesNumber<<  " " << edgeCount << " " << edgeList.size()<< endl;
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

bool FileHandler::maxEdgesPerNode(const vector<Edge> edgeList, const unsigned int id, unsigned int nodesNumber){
    for(unsigned int i=0;i < edgeList.size(); i++){
    //cout <<"ID: "<< id << "EDGE:" << nodesNumber << " " << edgeList.at(i).getFromNodeId() << "<->"<< edgeList.at(i).getToNodeId() << endl;
        if(edgeList.at(i).getFromNodeId() == id || 
           edgeList.at(i).getToNodeId() == id){
               nodesNumber--;
        }
    }
    //cout << "END Count: " << nodesNumber << endl;
    if(nodesNumber == 0){
        return true;
    }
    return false;
}

/*
 * This function helps to write a file to the directory
 */
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
