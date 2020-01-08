
#ifndef NODEAPP_HPP
#define NODEAPP_HPP

#include <string>
#include <thread>
#include <mutex>
#include "messageHandler.hpp"

#define BUFF_SIZE 256
#define MAX_WORKER 128

namespace Uebung1{

    class NetworkApp{

        private:
            // Classattributes
            std::string programName;
            unsigned int port;
            std::string nodeFileName;
            unsigned int neighborSize;
            std::string graphvizFileName;
            unsigned int doneNumber;
            unsigned int believeingNodes;
            unsigned int unbelieveingNodes;
            unsigned int maxSend;
            unsigned int minTrust;
            Uebung1::FileHandler handler;
            Uebung1::Node node;
            MessageHandler* messageHandler;
            std::mutex workerMutex;
            std::mutex doneMutex;
            std::mutex believeMutex;
            std::mutex unbelieveMutex;
        public: 
            // Constructors
            NetworkApp(const std::string programName,const unsigned int port, const std::string nodeFileName, 
                        const unsigned int neighborSize, const unsigned int maxSend, const unsigned int minTrust);
            NetworkApp(const std::string programName,const unsigned int port, const std::string nodeFileName, 
                        const std::string graphvizFileName, const unsigned int maxSend, const unsigned int minTrust);
            
            // Deconstructors

            // Inline-Elemtfunctions
            std::string getProgramName() const { return programName; }
            void setProgramName(std::string programName){ this->programName = programName; }
            
            std::string getNodeFileName() const { return nodeFileName; }
            void setNodeFileName(std::string nodeFileName){ this->nodeFileName = nodeFileName; }
            
            unsigned int getNeighborSize() const { return neighborSize; }
            void setNeighborSize(unsigned int neighborSize){ this->neighborSize = neighborSize; }

            // Memberfunctions
            void start();
            std::string messageDialog();
            void executeListingThread();        

        private:
            void reset();
            void executeWorkerThread(int socketFd);
            void executeStartNodeThread(unsigned int nodeId);
            void initTcpSocket(int& socketFd, unsigned int port);
    };
}
#endif // NODEAPP_HPP