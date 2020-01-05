#include <iostream>
#include <sstream>
#include "fileHandler.hpp"

using namespace std;
using namespace Uebung1;

int main(int argc, char** argv){
    if(argc != 4){
        cout << "usage: ./" << argv[0]  << " [NodeFileName] [OutputFileName] [EdgeCount]" << endl;
        return EXIT_FAILURE;
    }
    string nodeFile(argv[1]);
    string fileName(argv[2]);
    unsigned int edgeCount = atoi(argv[3]);
    FileHandler handler(nodeFile);
    handler.graphgen(fileName, edgeCount);
    return EXIT_SUCCESS;
}