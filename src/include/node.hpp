
#ifndef NODE_HPP
#define NODE_HPP

#include <iostream>
#include <string>
#include <vector>
#include "fileHandler.hpp"
#include "message.hpp"

namespace Uebung1{
  class FileHandler;
  class Node{
    private:
      // Classattributes
      unsigned int id;
      std::string ipAddress;
      unsigned int port;
      unsigned int maxNeighbor;
      Uebung1::Node* initNode;
      Uebung1::FileHandler* fileHandler;
      std::vector<Uebung1::Node> neighbors;      
      unsigned int maxSend;
      unsigned int believerEpsilon;

    public:
      // Constructors  
      Node();
      Node(const unsigned int id);  
      Node(const unsigned int id, const std::string ipAddress, const unsigned int port);
      Node(const unsigned int id, const unsigned int initNodeId, const std::string nodeFile, 
            const unsigned int maxNeighbor, const unsigned int maxSend, const unsigned int believerEpsilon);
      Node(const unsigned int id, const unsigned int initNodeId, const std::string nodeFile, 
            const std::string graphizFile, const unsigned int maxSend, const unsigned int believerEpsilon);
      Node(const Node& node);
      Node& operator= (const Node& node){
        return *this;
      }
      // Deconstructors
      // ~Node(){
      //   delete initNode;
      //   delete fileHandler;
      // }
      
      // Inline-Elementfunctions
      unsigned int getId() const { return id; }
      void setId(unsigned int id){ this->id = id; }
      
      std::string getIpAddress() const { return ipAddress; }
      void setIpAddress(std::string ipAddress){ this->ipAddress = ipAddress; }

      unsigned int getPort() const { return port; }
      void setPort(unsigned int port){ this->port = port; }

      Uebung1::Node* getInitNode() { return initNode; }
      void setInitNode(Uebung1::Node* initNode) { this->initNode = initNode; }

      std::vector<Node> getNeighbors() { return neighbors; }
      void setNeighbors(std::vector<Node> neighbors){ this->neighbors = neighbors; }

      unsigned int getBelieverEpsilon() const { return believerEpsilon; }
      void setBelieverEpsilon(unsigned int believerEpsilon){ this->believerEpsilon = believerEpsilon; }

      // Memberfunctions
      void startHandle();
      void selectNeighbors();
      void startCommunication();
      void sendOwnIdMessage(Message msg);
      std::string toString() const;
 
    private:
      bool hasNeighbor(const unsigned int id);
      void initTcpSocket(int& socketFd, unsigned int port);
      void executePingThread();
      void executeWorkerThread(int socketFd);
      void executeComHandleThread(std::string message);
      void executeSendMessageThread(Message message, Node node);
  };
}
#endif // NODE_HPP