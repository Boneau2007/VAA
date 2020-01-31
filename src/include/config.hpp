//
// Created by alhuber on 28.01.20.
//

#ifndef VAA_CONFIG_HPP
#define VAA_CONFIG_HPP

#include <string>

namespace Uebung1{
    class Config {
    private:
        std::string fileName;
        std::string initIpAddress;
        unsigned int initPort;
        std::string programName;
        //node config
        unsigned int maxNumOfNodes;
        std::string nodeFileName;
        //topologie config
        unsigned int maxRandNumber;
        unsigned int numberOfEdges;
        std::string graphvizFileName;
        std::string timesFile;
        unsigned int maxSend;
        unsigned int minTrust;
        //philisoph config
        unsigned int starterNumber;
        unsigned int roundNeighborsNumber;
        unsigned int maxRoundsNumber;
        unsigned int randElectionNumber;
    public:
        Config();
        explicit Config(std::string configFile);
        std::string getInitIpAddress() const {return initIpAddress; }
        unsigned int getInitPort() const { return initPort; }
        std::string getProgramName() const { return programName; }
        unsigned int getMaxNumOfNodes() const { return maxNumOfNodes; }
        std::string getNodeFileName() const { return nodeFileName; }
        unsigned int getMaxRandNumber() const { return maxRandNumber; }
        unsigned int getNumberOfEdges() const { return numberOfEdges; }
        std::string getGraphvizFileName() const { return graphvizFileName; }
        std::string getTimesFile() const { return timesFile; }
        unsigned int getMaxSend() const { return maxSend;}
        unsigned int getMinTrust() const { return minTrust; }
        unsigned int getStarterNumber() const { return starterNumber; }
        unsigned int getRoundNeighborsNumber() const { return roundNeighborsNumber; }
        unsigned int getMaxRoundsNumber() const { return maxRoundsNumber; }
        unsigned int getRandElectionNumber() const { return randElectionNumber; }
        };
}

#endif //VAA_CONFIG_HPP
