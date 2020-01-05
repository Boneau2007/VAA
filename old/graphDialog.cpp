#include <fstream>
#include <regex>
#include <cstdlib>  //Rand()
#include "graphDialog.hpp"


using namespace std;
using namespace Uebung1;


GraphDialog::GraphDialog(){
    graph = new Graph();
}
GraphDialog::GraphDialog(const string nodeFileName){
    graph = new Graph();
    readNodesFromFile(nodeFileName);
}
GraphDialog::GraphDialog(const string nodeFileName, const string formatFileName)
    : fileName(formatFileName) {
    graph = new Graph();
    readNodesFromFile(nodeFileName);
    setTopologie();
}

GraphDialog::GraphDialog(const string nodeFileName, const unsigned int neighborsNumber)
    : fileName("graphiz.dot"){
    graph = new Graph();
    readNodesFromFile(nodeFileName);
    setTopologie(neighborsNumber);
}

void GraphDialog::setTopologie(){
    const regex nameLine(R"(graph ([a-zA-z]+) \{)");
    const regex graphvizNodeLine(R"(([0-9]+) -- ([0-9]+);)");
    const regex endLine(R"(})");
    if(fileName.empty()){
        throw  runtime_error("ERROR: file name empty");
    }
    ifstream fileStream;
    fileStream.open(fileName, ios::out);
    if(!fileStream.is_open()){
        throw  runtime_error("Can't open file");
    }else{
        if(fileStream.peek() == ifstream::traits_type::eof()){
                throw runtime_error("ERROR: Empty file ");
        }
        string::size_type sz;
        string line;
        smatch matches;
        while(getline(fileStream, line)){
            if(regex_match(line, matches, nameLine)){
                graph->setName(matches.str(1));
            }else if(regex_match(line, matches, graphvizNodeLine)){
                unsigned int nodeIdFrom = stoi(matches.str(1), &sz);
                unsigned int nodeIdTo = stoi(matches.str(2), &sz);
                //cout << nodeIdFrom << " -- " << nodeIdTo << endl;
                Node* from = graph->getNode(nodeIdFrom);
                Node* to = graph->getNode(nodeIdTo);
                getGraph().addEdge(from, to, 1);
                if(!from->contains(*to)){
                    from->addNeighbor(to);
                }
                if(!to->contains(*from)){
                    to->addNeighbor(from);
                }
            }else if(regex_match(line, matches, endLine)){
            }else{
                fileStream.close();
                throw runtime_error("ERROR: No matching regex");
            }
        }
        fileStream.close();
    }
}

void GraphDialog::setTopologie(const unsigned int neighborSize){
    auto graphSize = graph->getNodeList().size();
    if(graphSize <= neighborSize){
        throw runtime_error ("Error, nodes has to be more than requested edges. Max edges are " + graphSize-1);
    }
    srand (time(NULL));
    for(unsigned int i=0;i < graphSize;i++){
        Node* node = graph->getNodeList().at(i);
        // cout << node->toString();
        while(node->getNeighbors().size() != neighborSize){
            // cout << node->getNeighbors().size() << endl;
            int num = rand()%graphSize;
            Node* randNode = graph->getNodeList().at(num);
            if(randNode->getId() != node->getId() && !node->contains(*randNode)){
                // cout << "   " <<randNode->toString();
                graph->addEdge(node, randNode, 1);
                if(!node->contains(*randNode)){
                    node->addNeighbor(randNode);
                }
                //randNode->addNeighbor(node); //Keine doppelten Kanten
            }
        }
    }
    if(fileName.empty()){
        throw  runtime_error("ERROR: file name empty");
    }
    ofstream fileStream(fileName);
    if(!fileStream.is_open()){
        throw  runtime_error("Can't open file");
    }else{
        fileStream.write(graph->toString().c_str(), graph->toString().size());
        fileStream.close();
    }
    //cout << graph->toString();
}


void GraphDialog::graphgen(const string fileName, const unsigned int nodeCount, const unsigned int edgeCount){
    if(edgeCount<nodeCount){
        throw runtime_error("ERROR: Less Edges");
    }
    
    for(unsigned int i=0; i < nodeCount; i++){
        Node* node = new Node(i);
        getGraph().addNode(node);
    }

    while(getGraph().getAdjList().size() != edgeCount){
        for(unsigned int i=0; i < nodeCount; i++){
            unsigned int randNodeId = rand()%nodeCount+1;
            while(i == randNodeId){
                randNodeId = rand()%nodeCount+1;
            }
            if(!graph->contains(*graph->getNodeList().at(i), *graph->getNodeList().at(randNodeId))){
                graph->addEdge(graph->getNodeList().at(i), graph->getNodeList().at(randNodeId),1);
            }
        }
    }

    if(fileName.empty()){
        throw  runtime_error("ERROR: file name empty");
    }
    ofstream fileStream;
    fileStream.open(fileName, ios::in);
    if(!fileStream.is_open()){
        throw  runtime_error("Can't open file");
    }else{
        fileStream.write("graph G {\n",11);
        for(unsigned int i=0;i < getGraph().getAdjList().size();i++){
            Edge& edge = *graph->getAdjList().at(i);
            ostringstream stream;
            stream << edge.getFromNode()->getId() << "--" << edge.getToNode()->getId() << ";" << endl;
            fileStream.write( stream.str().c_str(), stream.str().size());
        }
        fileStream.write("}",1);
        fileStream.close();
    }
}
