
#ifndef NODE_HPP
#define NODE_HPP

#include <iostream>
#include <string>
#include <vector>
#include "fileHandler.hpp"
#include "IMessage.hpp"
#include "messageHandler.hpp"
#include "configHandler.hpp"

namespace Handler{
    class FileHandler;
    class MessageHandler;
}

//namespace Messages{
//    class Messages::IMessage ;
//}

namespace Graph{
    class Node {
    private:
        // Classattributes
        unsigned int id;
        std::string ipAddress;
        unsigned int port;

        unsigned int recvRumors;
        bool hasSend;

        std::vector<Node> neighbors;
        time_t preferedTime;

        Handler::ConfigHandler config;
        Handler::FileHandler* fileHandler;
        Handler::MessageHandler* messageHandler;
        Node* initNode;
        Node* coordinator;

        bool waitAck;
    public:
        // Constructors
        Node();
        Node(const unsigned int id, std::string ipAddress, const unsigned int port);
        Node(const unsigned int id, const std::string& configFile, const bool isGraphviz);

        Node(const Node& node) {
            id = node.getId();
            ipAddress = node.getIpAddress();
            port = node.getPort();
            //recvRumors = node.getRecvRumors();
            //hasSend = node.getHasSend();
            //neighbors = node.getNeighbors();
            //preferedTime = node.getPreferedTime();
            //config = node.getConfig();
            //fileHandler = node.getFileHandler();
            //messageHandler = node.getMessageHandler();
            //doubleCounting = node.getDoubleCounting();
            //initNode = node.getInitNode();
            //coordinator = node.getCoordinator();
        }

        Node &operator=(const Node &node) {
            return *this;
        }

        // Deconstructors

        // Inline-Elementfunctions
        unsigned int getId() const { return id; }
        std::string getIpAddress() const { return ipAddress; }
        unsigned int getPort() const { return port; }
        std::vector<Node> getNeighbors() { return neighbors; }

        void setRecvRumors(unsigned int recvRumors) { this->recvRumors=recvRumors; }
        unsigned int getRecvRumors() const { return recvRumors; }

        void setHasSend(bool hasSend){ this->hasSend = hasSend;}
        //bool getHasSend() { return hasSend; }

        void setCoordinator(Node* coordinator) { this->coordinator=coordinator; }
        //Node* getCoordinator() { return coordinator; }

        //time_t getPreferedTime(){return preferedTime; }
        void setWaitAck(bool waitAck){this->waitAck = waitAck;}
        //MessageHandler* getMessageHandler(){ return messageHandler;}
        Handler::ConfigHandler getConfig() { return config; }
        Node* getInitNode(){return initNode; }
        Handler::FileHandler* getFileHandler(){return fileHandler; }

        // Memberfunctions
        void startHandle();
        void selectNeighbors();
        void incrementRecvRumors() { recvRumors++; }
        
        void sendMessageToNode(Messages::IMessage* message, const Node &targetNode);
        void sendToNeighbors(Messages::IMessage* msg);
        void sendToNeighborsExceptSource(Messages::IMessage* msg);

    private:
        void selectTime();
        static bool hasNeighbor(const std::vector<Node>& neighbors, const unsigned int id);
        void initTcpSocket(int &socketFd);

        //Account functions
        void acquireLock(unsigned int waitTillTime, unsigned int randIndex);
        void criticalSection(unsigned int randIndex);
        void loseLock();

        //Thread execution
        void executeWorkerThread(int socketFd);
        void executeSendMessageThread(Messages::IMessage* message, const Node &node);
        void executeAccountAlgorithmThread();

    };
}
#endif // NODE_HPP