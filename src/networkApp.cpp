
#include <iostream>
#include <netdb.h> //C
#include <bits/stdc++.h> // C      
#include <unistd.h> // C
#include <fcntl.h> // C
#include "networkApp.hpp"
#include "fileHandler.hpp"

using namespace std;
using namespace Uebung1;

const unsigned int INITIATOR = 1;
const unsigned int RUMOR = 2;
const unsigned int REMOTE_SHUTDOWN = 3;
const unsigned int SHUTDOWN_ALL = 0;

unsigned int receivedRumors = 0;
unsigned int trustMinimum = 0;
unsigned int maxEdges = 0;

NetworkApp::NetworkApp(const string programName, const unsigned int port, const string nodeFileName, 
                         const unsigned int neighborSize, const unsigned int maxSend, const unsigned int minTrust)
    :  programName(programName), port(port), nodeFileName(nodeFileName), neighborSize(neighborSize), 
        maxSend(maxSend), minTrust(minTrust), handler(FileHandler(nodeFileName)){
}

NetworkApp::NetworkApp(const string programName, const unsigned int port, const string nodeFileName, 
                        const string graphvizFileName, const unsigned int maxSend, const unsigned int minTrust)
    :  programName(programName), port(port), nodeFileName(nodeFileName), graphvizFileName(graphvizFileName),
        maxSend(maxSend), minTrust(minTrust),handler(FileHandler(nodeFileName)){
}

string NetworkApp::messageDialog(){
    ostringstream stream;
    stream << INITIATOR             << ". Initiat node" << endl;
    stream << RUMOR                 << ". Tell a rumor" << endl;
    stream << REMOTE_SHUTDOWN       << ". Terminate node" << endl;
    stream << SHUTDOWN_ALL          << ". Terminate all nodes" << endl;
    stream <<                          "Input: ";
    return stream.str();  
}

void NetworkApp::executeListingThread(){
    vector<thread> threadPool;
    int listenFd = -1;
    struct sockaddr_in address;
    int addrLenght = sizeof(address);
    initTcpSocket(listenFd, port);
    while(true){
        unique_lock<mutex> lock(workerMutex);
        int workerFd = accept(listenFd,(struct sockaddr *)&address, (socklen_t *)&addrLenght);
        threadPool.push_back(thread(&NetworkApp::executeWorkerThread, this, workerFd));
        lock.unlock();
    }
    for(unsigned int i=0; i < threadPool.size();i++){
        threadPool.at(i).join();
    }
}

void NetworkApp::executeWorkerThread(int socketFd){
    char buff[256];
    try{
        read(socketFd, buff, 256);
    }catch(exception& e){
            cerr << e.what() << endl;
    }
    string sBuff(buff);
    Message message(sBuff);
    if(message.getContent().find("done")){
        unique_lock<mutex> lock(doneMutex);
        doneNumber++;
        lock.unlock();
    }else if(message.getContent().find("true")){
        unique_lock<mutex> lock(believeMutex);
        believeingNodes++;
        lock.unlock();
    }else if(message.getContent().find("false")){
        unique_lock<mutex> lock(unbelieveMutex);
        unbelieveingNodes++;
        lock.unlock();
    }else{
        cout << "Unknown Cotrol content: " << message.getContent() << endl;
    }
    if(believeingNodes+unbelieveingNodes == handler.getNodeList().size()){
        cout << "Believing Nodes : " << believeingNodes << endl;
        cout << "Unbelieving Nodes : " << unbelieveingNodes << endl;
        reset();
    }
    if(doneNumber == handler.getNodeList().size()){
        Message message(0U, MESSAGE_TYPE::CONTROL, "evaluate");
        for(unsigned int i=0;i < handler.getNodeList().size();i++){
            messageHandler.sendMessage(message, handler.getNodeList().at(i));
        }
    }
    
}

void NetworkApp::reset(){
    doneNumber = 0;
    believeingNodes = 0;
    unbelieveingNodes = 0;
}
void NetworkApp::executeStartNodeThread(unsigned int nodeId){
    
}
void NetworkApp::start(){
    unsigned int nodeId = -1;
    int command = -1;
    thread listenThread = thread(&NetworkApp::executeListingThread, this);
    for(unsigned int i = 0;i < handler.getNodeList().size(); i++){
        ostringstream stream;
        stream << programName.c_str() << " " << handler.getNodeList().at(i).getId() << " " << port << " " << nodeFileName << " " << maxSend << " " << minTrust;
        graphvizFileName.empty() ? stream << " " << neighborSize : stream<< " "  << graphvizFileName;
        pid_t pid = fork();
        if(pid==0){
            system(stream.str().c_str());
            exit(EXIT_SUCCESS);
        }else if(pid < 0){
            throw new runtime_error("ERROR: fork() failed");
        }
    } 
    sleep(3);
    while(true){
        try{
            sleep(1);
            cout << messageDialog();
            cin >> command;
            if(command==INITIATOR || command==REMOTE_SHUTDOWN || command==SHUTDOWN_ALL || command==RUMOR ){
                Message message(0, MESSAGE_TYPE::CONTROL);
                if(command!=SHUTDOWN_ALL){
		    	    cout << "Enter Id: ";
		    	    cin >> nodeId;
                }
                if(command >= 0){
                    Node recvNode = handler.getNodeFromFile(nodeId); 
                    cout << recvNode.toString();
                    switch(command){
                        case INITIATOR: message.setContent("initiator"); 
                                        messageHandler.sendMessage(message, recvNode);
                                        break;
                        case REMOTE_SHUTDOWN: message.setContent("end");
                                              messageHandler.sendMessage(message,recvNode);
                                              break;
                        case SHUTDOWN_ALL:  message.setContent("end");
                                            for(int i=0;handler.getNodeList().size();i++){
                                                messageHandler.sendMessage(message, handler.getNodeList().at(i));
                                            }
                                            listenThread.join();
                                            exit(EXIT_SUCCESS);
                        case RUMOR: message.setContent("rumor");
                                    messageHandler.sendMessage(message,recvNode);
                                    break;
                        default: cout << "Unknown command" << endl;
                    }
                }
                
            }
        }catch (exception& e){
            
        }
       
    }
    listenThread.join();
}

int main(int argc, char* argv[]){
    if(argc != 7){
        cout << "usage: ./" << argv[0] << "networkPort nodeAppPath nodeFileName (nodeNumbers|graphizFileName) maxSend minTrust" << endl;
        exit(EXIT_FAILURE);
    }
    //cout << "NODE: " << argv[0] << " " << argv[1] << " " << argv[2] << " " << argv[3] << " "<< argv[4] << endl;
    unsigned int port = atoi(argv[1]);
    string programName(argv[2]);
    string nodeFileName(argv[3]);
    unsigned int neighborSize = atoi(argv[4]);
    string graphvizFileName = "";
    if(neighborSize == 0){
        graphvizFileName = argv[4]; // is Filename
    }
    unsigned int maxSend = atoi(argv[5]);
    unsigned int minTrust = atoi(argv[6]);
    NetworkApp* a1;
    a1 = graphvizFileName.empty() ? new NetworkApp(programName, port, nodeFileName, neighborSize, maxSend, minTrust) :
                                    new NetworkApp(programName, port, nodeFileName, graphvizFileName, maxSend, minTrust);
    
    a1->start();
    delete a1;
    return EXIT_SUCCESS;
}


/*
 * @function	initTcpSocket
 * @abstract	Initialize Tcp-Socket
 * @discuss 	This function initializes the Socket for the Tcp communication
 * @param		port		Portnumber
 * @result		Results an EXIT_FAILURE or EXIT_SUCCESS
 */
void NetworkApp::initTcpSocket(int& socketFd, unsigned int port){
    struct sockaddr_in serverAddress;
    int yes = 1;
	if ((socketFd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        throw  runtime_error("Error: on socket create\n"); 
	}
    if (setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
        throw runtime_error("Error: setsockopt\n"); 
    }
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_port = htons(port);
	if (bind(socketFd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0){
        throw runtime_error("Error: on socket bind\n"); 
	}
    if (listen(socketFd, 0) < 0){
        throw runtime_error("Error: on socket listen\n"); 
	}
}