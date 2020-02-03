#include <iostream>
#include <sstream>
#include "Handler/fileHandler.hpp"

using namespace std;
using namespace Uebung1;

int main(int argc, char** argv){
    if(argc != 5){
        cout << "usage: ./" << argv[0]  << " [NodeFileName] [NumberOfNodes] [NumberOfEdges] [OutputFileName] " << endl;
        return EXIT_FAILURE;
    }

    const string nodeFile(argv[1]);
    const unsigned int nodeNumbers = atoi(argv[2]);
    const unsigned int edgeNumbers = atoi(argv[3]);
    const string fileName(argv[4]);
    cout << nodeFile << " " << nodeNumbers << " " << edgeNumbers << " " << fileName << endl;
    FileHandler handler(nodeFile);
    handler.readNodes(nodeNumbers);
    handler.graphgen(fileName, nodeNumbers, edgeNumbers);
    return EXIT_SUCCESS;
}