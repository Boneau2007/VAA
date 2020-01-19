#include <string>
#include "include/node.hpp"
using namespace std;
using namespace Uebung1;

bool hasNeighbor(vector<Node> neighbors, const unsigned int id){
    for(unsigned int i=0;i < neighbors.size();i++){
        if(neighbors.at(i).getId() == id){
            return true;
        }
    }
    return false;
}

vector<Node> selectNeighbors(const unsigned int id, const unsigned int maxNeighbor, const string nodeFileName, const unsigned int maxNodes){
    vector<Node> neighbors;
    vector<Node> nodesInFile = FileHandler(nodeFileName).readNodes(maxNodes);
    srand (time(NULL));
    for(unsigned int i=0;i < maxNeighbor;i++){
        bool foundNeighbor = false;
        while(!foundNeighbor){
            unsigned int num = rand()%nodesInFile.size(); 
            Node randNode = nodesInFile.at(num);
            if(randNode.getId() != id && !hasNeighbor(neighbors, randNode.getId())){
                neighbors.push_back(randNode);
                foundNeighbor = true;
            }
        }
    }
    return neighbors;
}

int main(int argc, char** argv){
    if(argc != 10){
        cout << "usage: ./" << argv[0] << " id ipAddress port initNodePort maxNodes nodeFileName (maxNeighbor|graphizFileName) maxSend minTrust " << endl;
        exit(EXIT_FAILURE);
    }else{
        const unsigned int id =  atoi(argv[1]);
        const string ipAddress = argv[2];
        const unsigned int port =  atoi(argv[3]);
        const unsigned int initNodePort =  atoi(argv[4]);
        const unsigned int maxNodes =  atoi(argv[5]);
        const string nodeFileName = argv[6];
        const unsigned int maxNeighbor = atoi(argv[7]);
        const unsigned int maxSend =  atoi(argv[8]);
        const unsigned int minTrust =  atoi(argv[9]);
        vector<Node> neighbors;
        if(maxNeighbor == 0){
            string graphvizFileName = argv[6]; // is Filename
            FileHandler handler(nodeFileName, graphvizFileName);
            neighbors = handler.readGraphviz(id, handler.readNodes(maxNodes));
        }else{
            neighbors = selectNeighbors(id, maxNeighbor, nodeFileName, maxNodes);
        }
        //cout << "id " << id <<  " ipAddr "<< ipAddress << " port "<< port << " initPort "<< initNodePort<<  endl;
        Node node(id, ipAddress, port, initNodePort, neighbors, maxSend, minTrust);
    }
}