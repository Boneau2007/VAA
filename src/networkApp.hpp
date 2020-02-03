
#ifndef NODEAPP_HPP
#define NODEAPP_HPP

#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include "Handler/Include/configHandler.hpp"
#include "Handler/Include/messageHandler.hpp"

#define BUFF_SIZE 256
#define MAX_WORKER 128
class NetworkApp{

    private:
        std::mutex doneMutex;
        std::mutex believeMutex;
        std::mutex unbelieveMutex;
        const std::vector<std::string> menueList = {"Terminate all nodes", "Initiat node", "Tell a rumor", "Start philosopher meeting", "Terminate node"};
        unsigned int doneNumber = 0;
        unsigned int believeingNodes = 0;
        unsigned int unbelieveingNodes = 0;

        // Classattributes
        std::string configName;
        bool useGraphviz;
        Handler::ConfigHandler config;
        Handler::FileHandler fileHandler;
        Graph::Node node;

    public:
        // Constructors
        NetworkApp(std::string  configName, bool useGraphviz);

        // Deconstructors

        // Inline-Elemtfunctions


        void start();
        std::string messageDialog();
        void executeListingThread();

    private:
        static bool contains(const std::vector<unsigned int>& list, unsigned int number);
        std::vector<unsigned int> getRandNodeIdList(unsigned int maxNumber);
        void reset();
        void executeWorkerThread(int socketFd);
        static void initTcpSocket(int& socketFd, unsigned int port);
};
#endif // NODEAPP_HPP