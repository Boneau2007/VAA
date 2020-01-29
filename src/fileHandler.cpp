#include <fstream>  // ifstream()
#include <regex>    // regex()
#include <utility>
#include "fileHandler.hpp"

using namespace std;
using namespace Uebung1;

FileHandler::FileHandler()
    : fileName(""){}

FileHandler::FileHandler(string fileName)
    : fileName(std::move(fileName)){
}

/*
 * This function parses a node file in a List of Nodes
 */
void FileHandler::readNodes(const unsigned int maxNodesToRead){
    string line;
    smatch matches;
    ifstream fileStream;
    const regex isNodeLineRegex(R"(([0-9]*) (.*):([0-9]*))");    
    if(fileName.empty()){
        throw  runtime_error("ERROR: file name empty");
    }
    fileStream.open(fileName, ios::out);
    if(!fileStream.is_open()){
        throw  runtime_error("Can't open file");
    }else{
        // Are there more lines in file and is the requested nodeListSize reached 
        while(nodeList.size() < maxNodesToRead){
            getline(fileStream, line);
            if(line.empty()){
                continue;
            }else if(regex_match(line, matches, isNodeLineRegex)){
                //cout << matches.str(1) << " " << matches.str(2)  << " " << matches.str(3) << endl;
                nodeList.emplace_back(Node(stoi(matches.str(1)), matches.str(2), (unsigned  int)stoi(matches.str(3))));
            }else{
                throw runtime_error("ERROR: No matching node line");
            }
        }
    }
    fileStream.close();
}


/*
 * This function parses a graphviz file and sets the neighbors to the given Id
 */
vector<Node> FileHandler::readGraphviz(const unsigned int id, const std::string& graphFile){
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
            if(regex_match(line, matches, nameLine) || regex_match(line, matches, endLine)){
                continue;
            }else if(regex_match(line, matches, graphvizNodeLine)){
                unsigned int fromId = stoi(matches.str(1), &sz);
                unsigned int toId = stoi(matches.str(2), &sz);
                if(fromId == id){
                    neighborList.push_back(getNodeFromFile(toId));
                }else if(toId == id){
                    neighborList.push_back(getNodeFromFile(fromId));
                }
            }else{
                fileStream.close();
                throw runtime_error("ERROR: No matching graph regex");
            }
        }
        fileStream.close();
        return neighborList;
    }
}

/*
 * This function helps to find a Node in the nodeList with a given id
 */
Node FileHandler::getNodeFromFile(const unsigned int id){
    for(auto & i : nodeList){
        if(i.getId() == id){
            return i;
        }
    }
    throw runtime_error("Node not found");
}


/*
 * This function generates a graphviz file with a given name, the nodeList and the number of edges
 */
void FileHandler::graphgen(const string& outfileName, const unsigned int nodesNumber, const unsigned int edgeCount){
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
    writeGraphvizFile(outfileName, edgeList);
}

bool FileHandler::containsEdge(const vector<Edge>& edgeList, const unsigned int idFrom, const unsigned int idTo){
    for(auto i : edgeList){
            if( (i.getFromNodeId() == idFrom && i.getToNodeId() == idTo) ||
                (i.getToNodeId() == idFrom && i.getFromNodeId() == idTo)){
                   return true;
            }
    }
    return false;
}

bool FileHandler::maxEdgesPerNode(const vector<Edge>& edgeList, const unsigned int id, unsigned int nodesNumber){
    for(auto i : edgeList){
        if(i.getFromNodeId() == id ||
           i.getToNodeId() == id){
               nodesNumber--;
        }
    }
    //cout << "END Count: " << nodesNumber << endl;
    return nodesNumber == 0;
}

/*
 * This function helps to write a file to the directory
 */
void FileHandler::writeGraphvizFile(const string& fileName, const vector<Edge>& edgeList){
ostringstream stream;
    if(fileName.empty()){
        throw  runtime_error("ERROR: file name empty");
    }
    ofstream fileStream(fileName, ios::trunc);
    if(!fileStream.is_open()){
        throw  runtime_error("Can't open file");
    }else{
        stream << "graph G {" << endl; 
        for(auto i : edgeList){
            stream << i.toString() << endl;
        }
        stream << "}";
        fileStream.write(stream.str().c_str(), stream.str().size());
        fileStream.close();
    }
}
