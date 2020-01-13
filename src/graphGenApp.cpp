#include <iostream>
#include <sstream>
#include "fileHandler.hpp"

using namespace std;
using namespace Uebung1;

int main(int argc, char** argv){
    if(argc != 5){
        cout << "usage: ./" << argv[0]  << " [NodeFileName] [OutputSize] [EdgeCount] [OutputFileName] " << endl;
        return EXIT_FAILURE;
    }

    const string nodeFile(argv[1]);
    const unsigned int outputSize = atoi(argv[2]);
    const unsigned int edgeCount = atoi(argv[3]);
    const string fileName(argv[4]);

    FileHandler handler(nodeFile);
    handler.graphgen(fileName, handler.readNodes(outputSize), edgeCount);
    return EXIT_SUCCESS;
}