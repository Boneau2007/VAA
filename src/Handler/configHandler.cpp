//
// Created by alhuber on 28.01.20.
//

#include <istream>
#include <fstream>
#include <regex>
#include <iostream>
#include "Include/configHandler.hpp"

using namespace std;
using namespace Handler;
ConfigHandler::ConfigHandler() : initIpAddress(""), initPort(0), programName(""), maxNumOfNodes(0), nodeFileName(""),
                                 maxRandNumber(0), graphvizFileName(""), timesFile(""), maxSend(0), minTrust(0),
                                 starterNumber(0), roundNeighborsNumber(0), maxRoundsNumber(0) {}

ConfigHandler::ConfigHandler(string fileName) : fileName(fileName) {
    string line;
    smatch matches;
    ifstream fileStream;
    const regex isNodeLineRegex(R"((.*) (.*))");
    if(fileName.empty()){
        throw  runtime_error("ERROR: file name empty");
    }
    fileStream.open(fileName, ios::out);
    if(!fileStream.is_open()){
        throw  runtime_error("Can't open file");
    }else{
        // Are there more lines in file and is the requested nodeListSize reached
        for(unsigned int i=0; getline(fileStream, line); i++){
            if(line.empty()){
                continue;
            }else if(regex_match(line, matches, isNodeLineRegex)){
                if(matches.str(1) == "initPort"){
                    initPort = stoi(matches.str(2));
                } else if (matches.str(1) == "initIpAddress"){
                    initIpAddress = matches.str(2);
                }else if (matches.str(1) == "programName"){
                    programName = matches.str(2);
                }else if (matches.str(1) == "maxNumOfNodes"){
                    maxNumOfNodes = stoi(matches.str(2));
                }else if (matches.str(1) == "nodeFileName"){
                    nodeFileName = matches.str(2);
                }else if (matches.str(1) == "maxRandNumber"){
                    maxRandNumber = stoi(matches.str(2));
                }else if (matches.str(1) == "numberOfEdges"){
                    numberOfEdges = stoi(matches.str(2));
                }else if (matches.str(1) == "graphvizFileName"){
                    graphvizFileName = matches.str(2);
                }else if (matches.str(1) == "timesFile"){
                    timesFile = matches.str(2);
                }else if (matches.str(1) == "maxSend"){
                    maxSend = stoi(matches.str(2));
                }else if (matches.str(1) == "minTrust"){
                    minTrust = stoi(matches.str(2));
                }else if (matches.str(1) == "starterNumber"){
                    starterNumber = stoi(matches.str(2));
                }else if (matches.str(1) == "roundNeighborsNumber"){
                    roundNeighborsNumber = stoi(matches.str(2));
                }else if (matches.str(1) == "maxRoundsNumber"){
                    maxRoundsNumber = stoi(matches.str(2));
                }else if (matches.str(1) == "randElectionNumber"){
                    randElectionNumber = stoi(matches.str(2));
                }
            }else{
                throw runtime_error("ERROR: No matching node line");
            }
        }
    }
    fileStream.close();
}

