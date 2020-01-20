
#include <iostream>
#include <netdb.h> //C
#include <bits/stdc++.h> // C      
#include <unistd.h> // C
#include <fcntl.h> // C
#include "networkApp.hpp"
#include "fileHandler.hpp"

using namespace std;
using namespace Uebung1;

NetworkApp::NetworkApp( const string programName, const unsigned int port, const string nodeFileName, 
                        const unsigned int nodeNumber, const unsigned int neighborSize, const unsigned int maxSend,
                        const unsigned int minTrust, std::string timesFile, 
                        const unsigned maxStartNumber, const unsigned int maxPhilosopherNumber)
    :   programName(programName), port(port), nodeFileName(nodeFileName), neighborSize(neighborSize), doneNumber(0),
        believeingNodes(0), unbelieveingNodes(0), nodeNumber(nodeNumber), maxSend(maxSend), minTrust(minTrust), timesFile(timesFile), maxStartNumber(maxStartNumber), maxPhilosopherNumber(maxPhilosopherNumber){
            nodeList = FileHandler(nodeFileName).readNodes(nodeNumber);
            messageHandler = new MessageHandler(new Node(0, "127.0.0.1", port));
}

NetworkApp::NetworkApp( const string programName, const unsigned int port, const string nodeFileName, const unsigned int nodeNumber,
                        const string graphvizFileName, const unsigned int maxSend, const unsigned int minTrust, std::string timesFile,
                        const unsigned maxStartNumber, const unsigned int maxPhilosopherNumber)
    :   programName(programName), port(port), nodeFileName(nodeFileName), graphvizFileName(graphvizFileName), doneNumber(0),
        believeingNodes(0), unbelieveingNodes(0), nodeNumber(nodeNumber), maxSend(maxSend), minTrust(minTrust), timesFile(timesFile), maxStartNumber(maxStartNumber), maxPhilosopherNumber(maxPhilosopherNumber){
            nodeList = FileHandler(nodeFileName).readNodes(nodeNumber);
            messageHandler = new MessageHandler(new Node(0, "127.0.0.1", port));
}

/*
 * This function gives the dialog options
 */
string NetworkApp::messageDialog(){
    ostringstream stream;
    for(unsigned int i=0; i < menueList.size();i++){
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


std::vector<unsigned int> NetworkApp::getRandNodeIdList(const unsigned int maxNumber){
    vector<unsigned int> randNodeIdList;
    srand (time(NULL));
    while (randNodeIdList.size() < maxNumber){
        unsigned int num = rand()%nodeList.size();
        for(unsigned int i=0; i < randNodeIdList.size(); i++){
            if(nodeList.at(num).getId() != randNodeIdList.at(i)){
                randNodeIdList.push_back(nodeList.at(num).getId());
            }
        }
    }
    return randNodeIdList;
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
            messageHandler->sendMessage(message, nodeList.at(i));
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
        stream << programName << " " << nodeList.at(i).getId() << " ";
        stream << nodeList.at(i).getIpAddress() << " ";
        stream << nodeList.at(i).getPort() << " " << port << " ";
        stream << nodeNumber << " " << nodeFileName << " "; 
        graphvizFileName.empty() ? stream << neighborSize : stream << graphvizFileName;
        stream << " " << maxSend << " " << minTrust << " " << timesFile << " " << maxStartNumber << " " << maxPhilosopherNumber;
        //cout << stream.str() << endl;
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

            if(command == 0){
                message.setContent("end");
                for(unsigned int i=0; i  < nodeList.size();i++){
                    messageHandler->sendMessage(message, nodeList.at(i));
                }
                _Exit(EXIT_SUCCESS);
            }else if(command == 3){
                message.setContent("election");
                srand (time(NULL));
                unsigned int num = rand()%nodeList.size();
                //Get a random subset of the nodeList with max num NodeIds 
                vector<unsigned int> randNodeIdList = getRandNodeIdList(num);
                for(unsigned int i=0; i < randNodeIdList.size(); i++){ 
                    for(unsigned int j=0; j  < nodeList.size(); j++){
                        if(randNodeIdList.at(i) == nodeList.at(j).getId()){
                            messageHandler->sendMessage(message, nodeList.at(j));
                        }
                    }
                }
            }

		    cout << "Enter Id: ";
		    cin >> nodeId;
            unsigned int i = 0;
            for(;i < nodeList.size();i++){
                if(nodeList.at(i).getId() == nodeId){
                    break;
                }
            }
            if(command == 1){
                message.setContent("initiator"); 
                messageHandler->sendMessage(message, nodeList.at(i));

            }else if(command == 2){
                message.setContent("rumor");
                messageHandler->sendMessage(message, nodeList.at(i));

            }else if(command == 4){
                message.setContent("end");
                messageHandler->sendMessage(message, nodeList.at(i));

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
    if(argc != 11){
        cout << "usage: ./" << argv[0] << " networkPort nodeAppPath nodeFileName maxNumOfNodes (neighborNumber|graphizFileName) maxSend minTrust timesFile maxStartNumber maxPhilosopherNumber" << endl;
        exit(EXIT_FAILURE);
    }
    //cout << "NODE: " << argv[0] << " " << argv[1] << " " << argv[2] << " " << argv[3] << " "<< argv[4] << endl;
    unsigned int port = atoi(argv[1]);
    string programName(argv[2]);
    string nodeFileName(argv[3]);
    unsigned int nodeNumber = atoi(argv[4]);
    unsigned int neighborSize = atoi(argv[5]);
    string graphvizFileName = "";
    if(neighborSize == 0){
        graphvizFileName = argv[5]; // is Filename
    }
    unsigned int maxSend = atoi(argv[6]);
    unsigned int minTrust = atoi(argv[7]);
    string timesFile = argv[8];
    unsigned int maxStartNumber = atoi(argv[9]);
    unsigned int maxPhilosopherNumber = atoi(argv[10]);
    NetworkApp* a1;
    a1 = graphvizFileName.empty() ? new NetworkApp(programName, port, nodeFileName, nodeNumber, neighborSize, maxSend, minTrust, timesFile, maxStartNumber, maxPhilosopherNumber) :
                                    new NetworkApp(programName, port, nodeFileName, nodeNumber, graphvizFileName, maxSend, minTrust, timesFile, maxStartNumber, maxPhilosopherNumber);
    a1->start();
    delete a1;
    return EXIT_SUCCESS;
}
