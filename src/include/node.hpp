
#ifndef NODE_HPP
#define NODE_HPP
#include <string>
#include <map>

using namespace std;

namespace Uebung1{
  class Node{
    private:
      unsigned int id;
      string ipAddress;
      unsigned int port;
      bool initiator;
      map<unsigned int,Node> neighborList;
     
    public:
      //Constructors
      Node(unsigned int id, string ipAddress, unsigned int port, bool initiator);
      Node(unsigned int id, string ipAddress, unsigned int port);
      Node();
    
      //Inline-Elementfunctions
      const unsigned int getId() const { return id; }
      void setId(unsigned int id){ this->id = id; }
      
      const string getIpAddress() const { return ipAddress; }
      void setIpAddress(string ipAddress){ this->ipAddress = ipAddress; }
      
      const unsigned int getPort() const { return port; }
      void setPort(unsigned int port){ this->port = port; }

      bool getInitiator() const { return initiator; }
      void setInitiator(bool initiator){ this->initiator = initiator; }
  
      const map<unsigned int, Node> getNeighbors() const { return neighborList; }
      void setNeighbors(map<unsigned int, Node> neighborList){ this->neighborList = neighborList; }
    
      //Memberfunctions
      Node getNeighbor(unsigned int id);
      void addNeighbor(Node neighbor);
      void removeNeighbor(unsigned int id);
      void updateNeighbor(unsigned int id, Node neighbor);
  };
}
#endif // NODE_HPP