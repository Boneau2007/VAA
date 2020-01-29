
#include <iostream>
#include <utility>
#include <netdb.h> //C
#include <bits/stdc++.h> // C      
#include <unistd.h> // C
#include <fcntl.h> // C
#include "networkApp.hpp"
#include "fileHandler.hpp"

using namespace std;
using namespace Uebung1;

NetworkApp::NetworkApp(string configName, bool useGraphviz)
        : configName(configName), useGraphviz(useGraphviz), config(configName),
          node(0, config.getInitIpAddress(), config.getInitPort()),
          fileHandler(config.getNodeFileName()){
    fileHandler.readNodes(config.getMaxNumOfNodes());
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
    struct sockaddr_in address{};
    int addrLenght = sizeof(address);
    initTcpSocket(listenFd, config.getInitPort());
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
    if(believeingNodes+unbelieveingNodes == fileHandler.getNodeList().size()){
        cout << "Believing Nodes : " << believeingNodes << endl;
        cout << "Unbelieving Nodes : " << unbelieveingNodes << endl;
        reset();
    }
    if(doneNumber == fileHandler.getNodeList().size()){
        Message message2(0U, MESSAGE_TYPE::CONTROL, "evaluate");
        for(auto & i : fileHandler.getNodeList()){
            node.sendMessageToNode(message2, i);
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
    for(auto & i : fileHandler.getNodeList()){
        ostringstream stream;
        stream << config.getProgramName() << " "<< i.getId() <<" " << configName << " " << useGraphviz;
        cout << stream.str() << endl;
        pid_t pid = fork();
        if(pid==0){
            system(stream.str().c_str());
            exit(EXIT_SUCCESS);
        }else if(pid < 0){
            throw runtime_error("ERROR: fork() failed");
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
                for(auto & i : fileHandler.getNodeList()){
                    node.sendMessageToNode(message, i);
                }
                _Exit(EXIT_SUCCESS);
            }else if(command == 3){
                message.setContent("election");
                srand (time(nullptr));
                //unsigned int num = rand()%nodeList.size()+1;
                //Get a random subset of the nodeList with max num NodeIds
                vector<unsigned int> randNodeIdList = getRandNodeIdList(config.getRandElectionNumber());
                cout << randNodeIdList.size() << endl;
                for(unsigned int i : randNodeIdList){
                    for(auto & j : fileHandler.getNodeList()){
                        if(i == j.getId()){
                            node.sendMessageToNode(message, j);
                        }
                    }
                }
                sleep(2);
                continue;
            }

		    cout << "Enter Id: ";
		    cin >> nodeId;
            unsigned int i = 0;
            for(;i < fileHandler.getNodeList().size();i++){
                if(fileHandler.getNodeList().at(i).getId() == nodeId){
                    break;
                }
            }
            Node currentNode = fileHandler.getNodeList().at(i);
            if(command == 1){
                message.setContent("initiator");
                node.sendMessageToNode(message, currentNode);

            }else if(command == 2){
                message.setContent("rumor");
                node.sendMessageToNode(message, currentNode);

            }else if(command == 4){
                message.setContent("end");
                node.sendMessageToNode(message, currentNode);

            }else{
                cout << "Unknown command" << endl;
            }
        }catch (exception& e){
            cout << "Error: " << e.what() << endl;
        }
    }
}

/*
 * @function	initTcpSocket
 * @abstract	Initialize Tcp-Socket
 * @discuss 	This function initializes the Socket for the Tcp communication
 * @param		port		Portnumber
 * @result		Results an EXIT_FAILURE or EXIT_SUCCESS
 */
void NetworkApp::initTcpSocket(int& socketFd, unsigned int port){
    struct sockaddr_in serverAddress{};
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

bool NetworkApp::contains(const vector<unsigned int>& list, const unsigned int number){
    for(unsigned int i : list){
        if(i == number){
            return true;
        }
    }
    return false;
}

std::vector<unsigned int> NetworkApp::getRandNodeIdList(const unsigned int maxNumber){
    vector<unsigned int> randNodeIdList;
    srand (time(nullptr));
    while (randNodeIdList.size() < maxNumber){
        unsigned int num = rand()%fileHandler.getNodeList().size();
        cout << "Rand: " << num << endl;
        if(!contains(randNodeIdList, fileHandler.getNodeList().at(num).getId())){
            randNodeIdList.push_back(fileHandler.getNodeList().at(num).getId());
        }

    }
    return randNodeIdList;
}

int main(int argc, char* argv[]){
    if(argc != 3){
        cout << "usage: ./" << argv[0] << "configFile.txt (0|>0)" << endl;
        exit(EXIT_FAILURE);
    }
    string configName(argv[1]);
    string useGraphviz(argv[2]);
    bool isG = stoi(useGraphviz);
    NetworkApp a1(configName, isG);
    a1.start();
    return EXIT_SUCCESS;
}
