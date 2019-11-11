#include <iostream>

using namespace std;

namespace Uebung1{
  class Node{
    private:
      unsigned int id;
      string ipAddress;
      unsinged int port;
     
     public:
      Node(unsigned int id, string ipAdrress, unsigned int port);
      
      //Inline-Elementfunctions
      unsigned int getId(){return id;}
      void setId(){this->id = id;}
      
      string getIpAddress{return ipAddress;}
      void setIpAddress{this->ipAddress = ipAddress;}
      
      unsinged int getPort{return port;}
      void setPort{this->port = port;}
  
  }
}
