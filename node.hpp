#include <string>
using namespace std;

namespace Uebung1{
  class Node{
    private:
      unsigned int id;
      string ipAddress;
      unsigned int port;
     
     public:
      Node(unsigned int id, string ipAddress, unsigned int port);
      Node();
      ~Node();
      //Inline-Elementfunctions
      unsigned int getId(){return id;}
      void setId(unsigned int id){this->id = id;}
      
      string getIpAddress(){return ipAddress;}
      void setIpAddress(string ipAddress){this->ipAddress = ipAddress;}
      
      unsigned int getPort(){return port;}
      void setPort(unsigned int port){this->port = port;}
  
  }
}
