
#ifndef NODE_HPP
#define NODE_HPP

#include <iostream>
#include <string>
#include <vector>
#include "fileHandler.hpp"
#include "message.hpp"
#include "messageHandler.hpp"
#include "config.hpp"

namespace Uebung1{
    class FileHandler;
    class Message;
    class MessageHandler;
    class Node {
    private:
        // Classattributes
        unsigned int id;
        std::string ipAddress;
        unsigned int port;

        unsigned int recvRumors;
        bool hasSend;

        std::vector<Uebung1::Node> neighbors;
        time_t preferedTime;

        Config config;
        FileHandler* fileHandler;
        MessageHandler* messageHandler;
        Node* initNode;
        Node* coordinator;
    public:
        // Constructors
        Node();
        Node(const unsigned int id, const std::string &ipAddress, const unsigned int port);
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
        bool getHasSend() { return hasSend; }

        void setCoordinator(Node* coordinator) { this->coordinator=coordinator; }
        Node* getCoordinator() { return coordinator; }

        time_t getPreferedTime(){return preferedTime; }
        
        MessageHandler* getMessageHandler(){ return messageHandler;}
        Config getConfig() { return config; }
        Node* getInitNode(){return initNode; }
        FileHandler* getFileHandler(){return fileHandler; }
        // Memberfunctions
        void startHandle();
        void selectNeighbors();
        void incrementRecvRumors() { recvRumors++; }
        
        void sendMessageToNode(Message message, const Node &targetNode);
        void sendToNeighbors(const Message &msg);
        void sendToNeighborsExceptSource(const Uebung1::Message &msg);
        void sendToSuperNode(const Message &msg);

        std::string toString() const;
    private:
        void selectTime();
        bool hasNeighbor(const std::vector<Node>& neighbors, const unsigned int id);
        bool hasNeighbor(const unsigned int id);
        void initTcpSocket(int &socketFd);
        void executeWorkerThread(int socketFd);
        void executeSendMessageThread(Message message, const Node &node);

    };
}
#endif // NODE_HPP
