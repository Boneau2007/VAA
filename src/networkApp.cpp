
#include <iostream>
#include <netdb.h> //C
#include <bits/stdc++.h> // C      
#include <unistd.h> // C
#include <fcntl.h> // C
#include "networkApp.hpp"
#include "filenodeList.hpp"

using namespace std;
using namespace Uebung1;

NetworkApp::NetworkApp( const string programName, const unsigned int port, const string nodeFileName, 
                        const unsigned int numOfNodesInFile, const unsigned int neighborSize, const unsigned int maxSend, const unsigned int minTrust)
    :   programName(programName), port(port), nodeFileName(nodeFileName), neighborSize(neighborSize), doneNumber(0),
        believeingNodes(0), unbelieveingNodes(0), maxSend(maxSend), minTrust(minTrust)){
            nodeList = new FilenodeList(nodeFileName).readNodes(numOfNodesInFile);
            messagenodeList = new MessagenodeList(new Node(0, "127.0.0.1", port));
}

NetworkApp::NetworkApp( const string programName, const unsigned int port, const string nodeFileName, const unsigned int numOfNodesInFile,
                        const string graphvizFileName, const unsigned int maxSend, const unsigned int minTrust)
    :   programName(programName), port(port), nodeFileName(nodeFileName), graphvizFileName(graphvizFileName), doneNumber(0),
        believeingNodes(0), unbelieveingNodes(0), maxSend(maxSend), minTrust(minTrust)){
            messagenodeList = new MessagenodeList(new Node(0, "127.0.0.1", port));
}

/*
 * This function gives the dialog options
 */
string NetworkApp::messageDialog(){
    ostringstream stream;
    for(int i=0;;i++){
        stream << i << ". " << menueList.at(i) << endl;
    }
    stream << "Input: ";
    return stream.str();  
}

/*
 * This function handles incomming messages for the initNode
 */
void NetworkApp::executeListingThread(){
    vector<thread> threadPool;
    int listenFd = -1;
    struct sockaddr_in address;
    int addrLenght = sizeof(address);
    initTcpSocket(listenFd, port);
    while(true){
        try{
            int workerFd = accept(listenFd,(struct sockaddr *)&address, (socklen_t *)&addrLenght);
            threadPool.push_back(thread(&NetworkApp::executeWorkerThread, this, workerFd));
        }catch(exception& e){
            cout << e.what() << endl;
        }
    }
    for(unsigned int i=0; i < threadPool.size();i++){
        threadPool.at(i).join();
    }
}


/*
 * This function exectues what should be done by the incomming message content
 */
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
    if(believeingNodes+unbelieveingNodes == nodeList.size()){
        cout << "Believing Nodes : " << believeingNodes << endl;
        cout << "Unbelieving Nodes : " << unbelieveingNodes << endl;
        reset();
    }
    if(doneNumber == nodeList.size()){
        Message message(0U, MESSAGE_TYPE::CONTROL, "evaluate");
        for(unsigned int i=0;i < nodeList.size();i++){
            messagenodeList->sendMessage(message, nodeList.at(i));
        }
    }
    
}

/*
 * This function resets the rumor evaluation
 */
void NetworkApp::reset(){
    doneNumber = 0;
    believeingNodes = 0;
    unbelieveingNodes = 0;
}


/*
 * This function is the entrypoint for the whole programm
 */
void NetworkApp::start(){
    unsigned int nodeId = -1;
    int command = -1;
    thread listenThread = thread(&NetworkApp::executeListingThread, this);
    for(unsigned int i = 0;i < nodeList.size(); i++){
        ostringstream stream;
        stream << programName.c_str() << " " << nodeList.at(i).getId() << " " << port << " " << nodeFileName << " " << maxSend << " " << minTrust;
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
            sleep(2);
            cout << messageDialog();
            cin >> command;
            Message message(0, MESSAGE_TYPE::CONTROL);

            if(command==SHUTDOWN_ALL){
                message.setContent("end");
                for(unsigned int i=0; i  < nodeList.size();i++){
                    messagenodeList->sendMessage(message, nodeList.at(i));
                }
                _Exit(EXIT_SUCCESS);
            }

		    cout << "Enter Id: ";
		    cin >> nodeId;
            Node recvNode = nodeList.getNodeFromFile(nodeId);
            if(command==INITIATOR){
                message.setContent("initiator"); 
                messagenodeList->sendMessage(message, recvNode);

            }else if(command==RUMOR){
                message.setContent("rumor");
                messagenodeList->sendMessage(message, recvNode);

            }else if(command==REMOTE_SHUTDOWN){
                message.setContent("end");
                messagenodeList->sendMessage(message, recvNode);

            }else{
                cout << "Unknown command" << endl;
            }
        }catch (exception& e){
            cout << "Error: " << e.what() << endl;
        }
    }
    listenThread.join();
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

int main(int argc, char* argv[]){
    if(argc != 8){
        cout << "usage: ./" << argv[0] << " networkPort nodeAppPath nodeFileName numOfNodesInFile (neighborNumber|graphizFileName) maxSend minTrust" << endl;
        exit(EXIT_FAILURE);
    }
    //cout << "NODE: " << argv[0] << " " << argv[1] << " " << argv[2] << " " << argv[3] << " "<< argv[4] << endl;
    unsigned int port = atoi(argv[1]);
    string programName(argv[2]);
    string nodeFileName(argv[3]);
    unsigned int numOfNodesInFile = atoi(argv[4]);
    unsigned int neighborSize = atoi(argv[4]);
    string graphvizFileName = "";
    if(neighborSize == 0){
        graphvizFileName = argv[4]; // is Filename
    }
    unsigned int maxSend = atoi(argv[5]);
    unsigned int minTrust = atoi(argv[6]);
    NetworkApp* a1;
    a1 = graphvizFileName.empty() ? new NetworkApp(programName, port, nodeFileName, numOfNodesInFile, neighborSize, maxSend, minTrust) :
                                    new NetworkApp(programName, port, nodeFileName, numOfNodesInFile, graphvizFileName, maxSend, minTrust);
    numOfNodesInFile, 
    a1->start();
    delete a1;
    return EXIT_SUCCESS;
}
