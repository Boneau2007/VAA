
#ifndef NODE_HPP
#define NODE_HPP

#include <iostream>
#include <string>
#include <vector>
#include "IMessage.hpp"
#include "messages.hpp"
#include "messageHandler.hpp"
#include "fileHandler.hpp"
#include "configHandler.hpp"
#include "lamportClock.hpp"

namespace Handler{
    class FileHandler;
    class MessageHandler;
}

namespace Graph{
    class Node {
    private:
        // Classattributes
        unsigned int id;
        std::string ipAddress;
        unsigned int port;
        //one request queue per node, identified by id
        std::map<unsigned  int, std::map<Messages::IMessage, std::vector<unsigned int>>> localRequestQueueList;
        LamportClock localClock;

        unsigned int recvRumors{};
        bool hasSend{};

        std::vector<Node> neighbors;
        time_t preferedTime{};

        Handler::ConfigHandler config;
        Handler::FileHandler* fileHandler;
        Handler::MessageHandler* messageHandler;
        Node* initNode;
        Node* coordinator{};

        bool waitAck{};
        bool hasDeadlock;
        unsigned int lockId;
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

        Handler::ConfigHandler getConfig() { return config; }
        Handler::FileHandler* getFileHandler(){return fileHandler; }
        Node* getInitNode(){return initNode; }

        unsigned int getId() const { return id; }
        unsigned int getLockId() const { return lockId; }
        std::string getIpAddress() const { return ipAddress; }
        unsigned int getPort() const { return port; }
        std::vector<Node> getNeighbors() { return neighbors; }
        unsigned int getRecvRumors() const { return recvRumors; }
        LamportClock getLocalClock() const {return localClock;}

        void setRecvRumors(unsigned int recvRumors) { this->recvRumors=recvRumors; }
        void setHasSend(bool hasSend){ this->hasSend = hasSend;}
        void setCoordinator(Node* coordinator) { this->coordinator=coordinator; }
        void setWaitAck(bool waitAck){this->waitAck = waitAck;}
        void setHasDeadlock(bool hasDeadlock){this->hasDeadlock = hasDeadlock;}

        // Memberfunctions
        void startHandle();
        void selectNeighbors();
        void incrementRecvRumors() { recvRumors++; }

        void sendMessageToNode(Messages::IMessage* message, const Node &targetNode);
        void sendToNeighbors(Messages::IMessage* msg);
        void sendToNeighborsExceptSource(Messages::IMessage* msg);
        bool putMessageToLocalRequestQueue(const Messages::IMessage& message);
        void putLockAckToLocalRequestQueue(const Messages::IMessage& message);
        bool receivedReplyFromAll(unsigned int randIndex);
        bool isLowestTimestamp(unsigned int randIndex);
        void popMessageFromLocalRequestQueue(const Messages::IMessage& message);

    private:
        void selectTime();
        static bool hasNeighbor(const std::vector<Node>& neighbors, const unsigned int id);
        void initTcpSocket(int &socketFd);

        //Account functions
        void acquireLock(unsigned int waitTillTime, unsigned int randIndex);
        void criticalSection(unsigned int randIndex);
        void loseLock(unsigned int randIndex);

        //Thread execution
        void executeWorkerThread(int socketFd);
        void executeSendMessageThread(const Node &node);
        void executeAccountAlgorithmThread();
    };
}
#endif // NODE_HPP
