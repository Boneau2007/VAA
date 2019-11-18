#include <string>
#include <map>

using namespace std;

namespace Uebung1{
  class Node{
    private:
      unsigned int id;
      string ipAddress;
      unsigned int port;
      map<unsigned int,Node> neighborList;
     
    public:
      //Constructors
      Node(unsigned int id, string ipAddress, unsigned int port);
      Node();
    
      //Inline-Elementfunctions
      unsigned int getId(){return id;}
      void setId(unsigned int id){this->id = id;}
      
      string getIpAddress(){return ipAddress;}
      void setIpAddress(string ipAddress){this->ipAddress = ipAddress;}
      
      unsigned int getPort(){return port;}
      void setPort(unsigned int port){this->port = port;}
  
      map<unsigned int, Node> getNeighbors(){return neighborList;}
      void setNeighbors(map<unsigned int, Node> neighborList){this->neighborList = neighborList;}
    
      //Memberfunctions
      Node getNeighbor(unsigned int id);
      void addNeighbor(Node neighbor);
      void removeNeighbor(unsigned int id);
      void updateNeighbor(unsigned int id, Node neighbor);
  };
}
