
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
      std::vector<Uebung1::Node> neighbors;
      unsigned int initNodePort;

      unsigned int maxSend;
      unsigned int believerEpsilon;
      unsigned int recvRumors;
      bool hasSend;

      time_t preferedTime;
      unsigned int virtualParentId;
      unsigned int startMessageNumber;
      unsigned int philosopherNumber;
      unsigned int voteCount;

    public:
      // Constructors  
      Node();
      Node(const unsigned int id);  
      Node(const unsigned int id, const std::string ipAddress, const unsigned int port);
      Node( const unsigned int ownId, const std::string ipAddress, const unsigned int port, const unsigned int initNodePort, 
            const std::vector<Node> neighbors, const unsigned int maxSend, const unsigned int believerEpsilon);
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

      unsigned int getInitPort() const { return initNodePort; }
      void setInitPort(unsigned int initNodePort){ this->initNodePort = initNodePort; }

      std::vector<Node> getNeighbors() { return neighbors; }
      void setNeighbors(std::vector<Node> neighbors){ this->neighbors = neighbors; }

      unsigned int getBelieverEpsilon() const { return believerEpsilon; }
      void setBelieverEpsilon(unsigned int believerEpsilon){ this->believerEpsilon = believerEpsilon; }

      unsigned int getRecvRumors() const { return recvRumors; }
      void setRecvRumors(unsigned int recvRumors) { this->recvRumors=recvRumors; }

      bool getHasSend(){ return hasSend; }
      void setHasSend(bool hasSend){ this->hasSend=hasSend; }

      unsigned int getVirtualParentId() const { return virtualParentId; }
      void setVirtualParentId(unsigned int virtualParentId) { this->VirtualParentIdvirtualParentId=virtualParentId; }

      // Memberfunctions
      void startHandle();
      void selectNeighbors();
      void startCommunication();
      void sendOwnIdMessage(Message msg);
      void incrementRecvRumors(){ recvRumors++; }
      std::string toString() const;
 
    private:
      bool hasNeighbor(const unsigned int id);
      void initTcpSocket(int& socketFd, unsigned int port);
      void executePingThread();
      void executeWorkerThread(int socketFd);
      void executeSendMessageThread(Message message, Node node);
  };
}
#endif // NODE_HPP