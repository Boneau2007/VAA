#include <string>
#include "include/node.hpp"
using namespace std;
using namespace Uebung1;

void Node::selectNeighbors(){
    if(fileHandler->getGraphFile().empty()){
        srand (time(NULL));
        for(unsigned int i=0;i < maxNeighbor;i++){
            bool foundNeighbor = false;
            while(!foundNeighbor){
                unsigned int num = rand()%fileHandler->getNodeList().size(); 
                Node randNode = fileHandler->getNodeList().at(num);
                if(randNode.getId() != id && !hasNeighbor(randNode.getId())){ 
                    neighbors.push_back(randNode);
                    foundNeighbor = true;
                }
            }
        }
    }else{
        neighbors = fileHandler->readGraphviz(id);
    }   
}

int main(int argc, char** argv){
    if(argc != 7){
        cout << "usage: ./" << argv[0] << " ownId initNodeId nodeFileName maxSend minTrust  (maxNodes|graphizFileName)" << endl;
        exit(EXIT_FAILURE);

    }else{
        unsigned int ownId =  atoi(argv[1]);
        unsigned int initNodePort =  atoi(argv[2]);
        string nodeFileName = argv[3];
        unsigned int maxSend =  atoi(argv[4]);
        unsigned int minTrust =  atoi(argv[5]);
        unsigned int maxNeighbor = atoi(argv[6]);
        string graphvizFileName = "";
        
        if(maxNeighbor == 0){
            graphvizFileName = argv[6]; // is Filename
            //cout << ownId << " " << initNodePort << " " << nodeFileName << " " << graphvizFileName << " " << maxSend << " " << minTrust; //<< " " << maxNeighbor << endl;
            Node node(ownId, initNodePort, nodeFileName, graphvizFileName, maxSend, minTrust);
        }else{
            Node node(ownId, initNodePort, nodeFileName, maxNeighbor, maxSend, minTrust);
        }
    }
}