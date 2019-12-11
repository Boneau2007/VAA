#include "include/nodeApp.hpp"

const string NODELINE_REGEX = "(([0-9]*) (.*):([0-9]*)(\n|))";
const string GRAPHVIZ_NAME_REGEX = "(graph ([a-zA-z]+) {)";
const string GRAPHVIZ_NODE_REGEX = "(([0-9]+) -- ([0-9]+);)";
const string GRAPHVIZ_END_REGEX = "}";
const static int EXIT = 0;
const static int TASK1 = 1;
const static int TASK2 = 2;
const static int TASK3 = 3;
const static int TASK4 = 4;

const unsigned int SHUTDOWN_ALL = 0;
const unsigned int REMOTE_SHUTDOWN = 1;
const unsigned int UPGRADE = 2;
const unsigned int TRIGGER_MESSAGE_SEND = 3;

Uebung1::Graph readGraphviz(string path){
    ifstream fileStream;
    fileStream.open(path, ios::out);
    if(!fileStream.is_open()){
        cout << "Error: Can't open file" << endl;
        std::exit(EXIT_FAILURE);
    }else{
        string line;
        smatch matches;
        const regex isNodeLineRegex(GRAPHVIZ_NODE_REGEX);
        const regex isNameLineRegex(GRAPHVIZ_NAME_REGEX);
        const regex isEndLineRegex(GRAPHVIZ_END_REGEX);
        string name;
        vector<Uebung1::edge> edgeList;
        vector<unsigned int> nodeList;
        for(int i=0; getline(fileStream, line); i++){
            if(line.empty()){
                cout << "Skip empty line";
                continue;
            }
            if(regex_match(line, matches, isNameLineRegex)){
                name = matches.str(1);
            }else if(regex_match(line, matches, isNodeLineRegex) && i != 0){
                unsigned int nodeFrom = atoi(matches.str(1).c_str());
                unsigned int nodeTo = atoi(matches.str(2).c_str());
                Uebung1::edge* newEdge = new Uebung1::edge();
                newEdge->fromNode = nodeFrom;
                newEdge->toNode = nodeTo;
                edgeList.push_back(*newEdge);
                if(find(nodeList.begin(), nodeList.end(), nodeFrom) != nodeList.end()){
                }else{
                    nodeList.push_back(nodeFrom);
                }
                if(find(nodeList.begin(), nodeList.end(), nodeTo) != nodeList.end()){
                }else{
                    nodeList.push_back(nodeTo);
                }
            }else if(regex_match(line, matches, isEndLineRegex)){
                fileStream.close();
                Uebung1::Graph graph(name, &edgeList[0], edgeList.size(), nodeList.size());
                return graph;
            }else{
                cout << "No matching Regex in line: " << line << endl;
                cout << "Please edit your file.";
                fileStream.close();
                std::exit(EXIT_FAILURE);
            }
        }
        throw new exception();
    }
}

void setHostAddress(struct sockaddr_in* host_addr, string host, unsigned int port){
  struct hostent* host_info;
  unsigned long address;
  if((address = inet_addr(host.c_str())) != INADDR_NONE){
    memcpy((char *)&host_addr->sin_addr, &address, sizeof(address));
  }else{
    host_info = gethostbyname(host.c_str());
    if(host_info==NULL){
      printf("\nUnknown Server");
      return;
    }else{
      memcpy((char *)&host_addr->sin_addr, &host_info->h_name, host_info->h_length);
    }
  }
  host_addr->sin_family = AF_INET;
  host_addr->sin_port = htons(port);
}

/*
 * @function	initTcpSocket
 * @abstract	Initialize Tcp-Socket
 * @discuss 	This function initializes the Socket for the Tcp communication
 * @param		port		Portnumber
 * @result		Results an EXIT_FAILURE or EXIT_SUCCESS
 */
int initTcpSocket(int port){
	int tcpSocket;
	if ((tcpSocket = createSocket(AF_INET, SOCK_STREAM, 0)) < 0){
		return -1;
	}else{
		clog << "Tcp-listener is now at fd : ["<< tcpSocket << "]" << endl;
	}if (bindSocket(&tcpSocket, INADDR_ANY, port) < 0){
		return -2;
	}else{
		clog << "Tcp-listener is now bound" << endl;
	}

	if (listenSocket(tcpSocket) < 0){
		return -3;
	}
	else{
		clog << "Tcp-listener is now in listen mode" << endl;
	}
	return tcpSocket;
}

int handleIncommingMessages(int socket, bool* idSend ,map<unsigned int, Uebung1::Node>* nodeList, unsigned int ownId){
	char* buff = (char*)calloc(BUFF_SIZE, sizeof(char));
    int messageLength = 0 ;
    if((messageLength = recv(socket, buff, BUFF_SIZE, 0)) == -1){
		switch (errno){
		case EWOULDBLOCK: fprintf(stderr,"\nOperation would block. Try again\n");
						  return EWOULDBLOCK;
		default: fprintf(stderr,"\nError, recv() failed.\n");
				 return -1;
		}
	}else{
        chrono::system_clock::time_point now = chrono::system_clock::now();
        time_t time = chrono::system_clock::to_time_t(now);
        cout << buff << " : " << asctime(gmtime(&time)) << endl;
        if(strncmp(buff,"end",3)){
            exit(EXIT_SUCCESS);
        }else if(strncmp(buff,"upgrade",7)){
            nodeList->at(ownId).setInitiator(true);
        }else if(strncmp(buff,"broadcast",9)){
            *idSend = false;
        }
        return EXIT_SUCCESS;
    }
}


map<unsigned int, Uebung1::Node> getNodesInFile(string path){
    map<unsigned int, Uebung1::Node> nodeList;
    ifstream fileStream;
    fileStream.open(path, ios::out);
    if(!fileStream.is_open()){
    cout << "Error: Can't open file" << endl;
    exit(EXIT_FAILURE);
    }else{
        string line;
        smatch matches;
        const regex isNodeLineRegex(NODELINE_REGEX);
        for(int i=0; getline(fileStream, line); i++){
            if(line.empty()){
                cout << "Skip empty line";
                break;
            }
            if(regex_match(line, matches, isNodeLineRegex)){
                unsigned int id = atoi(matches.str(1).c_str());
                string ipAddress(matches[3]);
                unsigned int port = atoi(matches.str(4).c_str());
                nodeList.insert(make_pair(id,Uebung1::Node(id, ipAddress, port, false)));
            }else{
                cout << "No matching Regex in line: " << line << endl;
                cout << "Please edit your file.";
                exit(EXIT_FAILURE);
            }
        }
        fileStream.close();
    }
    return nodeList;
}

int handleOutgoingMessages(int socket, sockaddr_in host_addr, string message){
	if (connect(socket, (struct sockaddr*) & host_addr, sizeof(host_addr)) == -1) {
        cout << "Could not connect to host" << endl;
		return EXIT_FAILURE;
	}
	write(socket, message.c_str(), message.size());
	closeSocket(socket);
    return EXIT_SUCCESS;
}

string menueDialog(){
    ostringstream stream;
    stream << TASK1 << ".Aufgabe" << endl; 
    stream << TASK2 << ".Aufgabe" << endl; 
    stream << TASK3 << ".Aufgabe" << endl; 
    stream << TASK4 << ".Aufgabe" << endl; 
    stream << EXIT  << ". Exit" << endl; 
    return stream.str();  
    }

    
string controlMessageDialog(){
    ostringstream stream;
    stream << SHUTDOWN_ALL          << ". Terminate all nodes" << endl;
    stream << REMOTE_SHUTDOWN       << ". Terminate one node" << endl;
    stream << UPGRADE               << ". Upgrade node to initiator" << endl;
    stream << TRIGGER_MESSAGE_SEND  << ". Trigger broudcast" << endl;
    return stream.str();  
}

int getInt(){
    long value;
    cout << "->" ;
    fflush(stdin);
    fgets((char*)&value, sizeof(value)-1, stdin);
    fflush(stdin);
    value = atoi((char*)&value);
    return value;
}

void communicationHandle(map<unsigned int,Uebung1::Node> nodeList, unsigned int ownId){
    struct sockaddr_in host;
    struct timeval time;
    time.tv_sec = 0;
    time.tv_usec = 10;
    int err;
    bool idSend = false;
	int tcpListener = -1;
	int workers[MAX_WORKER];
	for (int i = 0; i < MAX_WORKER; i++) {
		workers[i] = -1;
	}
    if((tcpListener = initTcpSocket(nodeList.at(ownId).getPort())) < 0){
        cout << "Process with: " << ownId << "exiting" << endl; 
        exit(tcpListener);
    }
    setsockopt(tcpListener, SOL_SOCKET, SO_RCVTIMEO, (const char*)&time, sizeof(time)); //set Timeout

    fd_set fdset;
	fcntl(tcpListener, F_SETFL, O_NONBLOCK);    //set listener in non-blocking mode
	if( nodeList.at(ownId).getInitiator()==true){
        cout << controlMessageDialog();
    }
    while(true){
        FD_ZERO(&fdset);
		FD_CLR(tcpListener, &fdset);
		FD_CLR(STDIN_FILENO, &fdset);
		FD_SET(tcpListener, &fdset);
        for (int i = 0; i < MAX_WORKER; i++) {
			if (workers[i] != -1) {
				FD_SET(workers[i], &fdset);
			}
		}
		FD_SET(STDIN_FILENO, &fdset);
interrupt:
		if ((err = select(FD_SETSIZE, &fdset, NULL, NULL, NULL) )< 0) {
			if (err < 0) {
        		switch (errno) {
        		case EINTR:     goto interrupt;
        		case EAGAIN:    break; /* empty rx queue */
        		case ETIMEDOUT: break; /* recv timeout */
        		case ENOTCONN:  break; /* not connected yet */
        		default:        printf("Connetction lost [%d]", err);
        		}
			}
		}
        if(FD_ISSET(STDIN_FILENO, &fdset)){
            if(nodeList.at(ownId).getInitiator()==true){
                int command;
                fgets((char*)&command, 8, stdin);
			    fflush(stdin);
  			    command = atoi((char*)&command);
                if(command==UPGRADE){
                    for(int i = 0;i < MAX_WORKER;i++){
                        if(workers[i] != -1){
                            char* cMessage = (char*)calloc(BUFF_SIZE, sizeof(char));
						    printf("\n Please enter Id ~>");
						    fgets(cMessage, BUFF_SIZE, stdin);
						    fflush(stdin);
                            setHostAddress(&host,nodeList.at(atoi(cMessage)).getIpAddress(),nodeList.at(atoi(cMessage)).getPort());        
                            handleOutgoingMessages(workers[i], host, "upgrade");
                            closeConnection(workers[i]);
                            nodeList.at(ownId).setInitiator(false);
                            free (cMessage);
                        }
                    }
                }else if(command == REMOTE_SHUTDOWN){
                    for(int i = 0;i < MAX_WORKER;i++){
					    if(workers[i] != -1){
                            char* cMessage = (char*)calloc(BUFF_SIZE, sizeof(char));
						    printf("\n Please enter Id ~>");
						    fgets(cMessage, BUFF_SIZE, stdin);
						    fflush(stdin);
                            setHostAddress(&host,nodeList.at(atoi(cMessage)).getIpAddress(),nodeList.at(atoi(cMessage)).getPort());
                            handleOutgoingMessages(workers[i], host, "end");
                            closeConnection(workers[i]);
                            free(cMessage);
                        }
                    }
                }else if(command == SHUTDOWN_ALL){
                    for(int i = 0;i < MAX_WORKER;i++){
					    if(workers[i] != -1){
                            for( const auto& node : nodeList ){
                                setHostAddress(&host,node.second.getIpAddress(),node.second.getPort());
                                handleOutgoingMessages(workers[i], host, "end");
                                closeConnection(workers[i]);
                            }
                        }
                    }
                    exit(EXIT_SUCCESS);
                }else if(command == TRIGGER_MESSAGE_SEND){
                    for(int i = 0;i < MAX_WORKER;i++){
			            if(workers[i] != -1){
                            for( const auto& node : nodeList ){
                                if(node.first == ownId){
                                    continue;
                                }
                                setHostAddress(&host,node.second.getIpAddress(),node.second.getPort());
                                handleOutgoingMessages(workers[i], host, "broadcast");
                                closeConnection(workers[i]);
                            }
                        }
                    }
                    idSend=false;
                }
                controlMessageDialog();
            }
        }else if (!idSend) {
            for (int i = 0; i < MAX_WORKER; i++) {
				if (workers[i] == -1) {
					for (map<unsigned int, Uebung1::Node>::iterator it = nodeList.begin(); it != nodeList.end(); ++it) {
				        setHostAddress(&host, it->second.getIpAddress(), it->second.getPort());
                                cout << host.sin_addr.s_addr << endl;
                                cout << host.sin_family << endl;
                                cout << host.sin_port << endl;
                                cout << host.sin_zero << endl;
				        handleOutgoingMessages(workers[i], host, to_string(it->second.getId()));
			        }
				}
			}
			idSend = true;
        //Incomming Connection
		}else if(FD_ISSET(tcpListener, &fdset)){
            for (int i = 0; i < MAX_WORKER; i++) {
				if (workers[i] == -1) {
				    acceptSocket(&tcpListener, &workers[i]);
			    }
			}
		}
		//IO-happens
		else {
            for (int i = 0; i < MAX_WORKER; i++) {
				if (FD_ISSET(workers[i], &fdset)) {
				    handleIncommingMessages(workers[i], &idSend, &nodeList, ownId);
                    closeConnection(workers[i]);
			    }
		    }
	    }
    }
}

void startNode(string nodeFileName, map<unsigned int, Uebung1::Node> neighborList, unsigned int ownId){
    int pid = fork();
    cout << "PID: " << pid << endl;
    if(pid==0){
        map<unsigned int,Uebung1::Node> nodeList = getNodesInFile(nodeFileName);
        for(unsigned int i=3;i < neighborList.size();i++){
            nodeList.at(ownId).setNeighbors(neighborList);
        }
        int fd = open("/dev/null", O_RDWR);
  dup2(fd, 0);
  dup2(fd, 1);
  dup2(fd, 2);
        communicationHandle(nodeList, ownId);
    }
}


int main(int argc, char* argv[]){

   ofstream out("log.txt");
   auto old_rdbuf = clog.rdbuf();
   clog.rdbuf(out.rdbuf());
    string programName(argv[0]);
    string nodeFileName;
    string graphvizFileName;
    unsigned int ownId;
    unsigned int neighborSize;
    string initiator;
    map<unsigned int,Uebung1::Node> nodeList;
    int iter=-1;
    while(iter != EXIT){
        switch(iter){
            case TASK1: {
                        cout << "Please enter node-filename: ";
                        getline(cin,nodeFileName);  
                        cout << "Please enter NodeId: ";
                        ownId = getInt();
                        cout << "Please enter number of neighbors: ";
                        neighborSize = getInt();
                        
                        nodeList = getNodesInFile(nodeFileName);
                        nodeList.at(ownId).setInitiator(true);
                        if(nodeList.size() <= neighborSize){
                            cout << "Not enough Nodes in file for Network : Is " << nodeList.size() << " Min: "<< neighborSize+1 << endl;
                            exit(EXIT_FAILURE);
                        }
                        for( const auto& node: nodeList ){
                            vector<Uebung1::Node> neighborList;
                            for( const auto& neighborNode : nodeList ){
                                if(node.second.getNeighbors().size() != neighborSize && node.first != neighborNode.first){
                                    neighborList.push_back(neighborNode.second);
                                }
                            }
                            for(unsigned int i=0;i < neighborList.size(); i++){
                                nodeList.at(node.first).addNeighbor(neighborList.at(i));
                            }
                        }
                        for( const auto& node : nodeList ){
                            if(node.second.getId() != ownId){
                                startNode(nodeFileName, nodeList.at(node.first).getNeighbors(), node.first);
                            }
                        }
                        communicationHandle(nodeList, ownId);
                        cout << menueDialog() << endl;
                        iter = getInt();
                        break;
                        }
            case TASK2: {
                        cout << "Please enter filename: ";
                        getline(cin,nodeFileName); 
                        cout << "Please enter graphviz-filename: ";
                        getline(cin,graphvizFileName); 
                        cout << "Please enter NodeId: ";
                        ownId = getInt();

                        Uebung1::Graph graph = readGraphviz(graphvizFileName);
                        cout << graph.toString();
                        for(unsigned int i=0; i < graph.getNodeCount(); i++){
                            Uebung1::adjNode* node = graph.getHead()[i];
                            vector<Uebung1::Node> neighborList;
                            while(node != nullptr){
                                nodeList.at(i).addNeighbor(nodeList.at(node->value));
                                node = node->next;
                            }
                        }
                        
                        for( const auto& node : nodeList ){
                            if(node.second.getId() != ownId){
                                startNode(nodeFileName, nodeList.at(node.first).getNeighbors(), node.first);
                            }
                        }
                        sleep(1);
                        communicationHandle(nodeList, ownId);
                        cout << menueDialog() << endl;
                        iter = getInt();
                        break;
                        }
            case TASK3: 
                        cout << menueDialog() << endl;
                        iter = getInt();
                        break;
            case TASK4: 
                        cout << menueDialog() << endl;
                        iter = getInt();
                        break;
            case EXIT:  cout << "Have a nice day :)!";
                        exit(EXIT_SUCCESS);
                        break;
            default:    cout << menueDialog() << endl;
                        iter = getInt();
                        break;
        }
    }
     std::clog.rdbuf(old_rdbuf);
}
