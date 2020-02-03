#include <string>
#include <fstream>  // ifstream()
#include "Graph/node.hpp"

using namespace std;
using namespace Graph;

int main(int argc, char** argv){
    if(argc != 4){
        cout << "usage: " << argv[0] << " id configFile isGraphviz" << endl;
        exit(EXIT_FAILURE);
    }else{
        Node node(stoi(argv[1]), argv[2], (bool)stoi(argv[3]));
    }
}