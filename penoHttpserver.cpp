#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <cstring>
#include <string>
#include <deque>
#include <map>
using namespace std;

#define _DEBUG		1

#define DEFAULT_SERVER_PORT	8080
#define CLIENTS_MAX	10

#define RECV_BUFF_SIZE	1000
#define CMD_BUFF_SIZE	260

#define PIPE_NULL	0
#define PIPE_IPC	1


void split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
}


vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}


int main(int argc, char *argv[]) {
	int sockfd, childfd, pid, server_port = DEFAULT_SERVER_PORT;
	unsigned int cli_len;
	struct sockaddr_in serv_addr, cli_addr;
	if(argc > 1){
		server_port = atoi(argv[1]);
	}

	chdir("/home/stream/UZNP/");
	chdir("./www");

	// Socket
	if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		cout << "[Server] Cannot create socket\n";
		exit(EXIT_FAILURE);
	}

	// Reuse address
	int ture = 1;
	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &ture, sizeof(ture)) < 0) {  
		cout << "[Server] Setsockopt failed\n";
		exit(EXIT_FAILURE);  
	}

	// Bind
	serv_addr.sin_family		= AF_INET;
	serv_addr.sin_addr.s_addr	= htonl(INADDR_ANY);
	serv_addr.sin_port			= htons(server_port);

	if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		cout << "[Server] Cannot bind address\n";
		exit(EXIT_FAILURE);
	}

	// Listen
	if(listen(sockfd, CLIENTS_MAX) < 0) {
		cout << "[Server] Failed to listen\n";
		exit(EXIT_FAILURE);
	}

	// Accept
	while(1) {
		if(_DEBUG)	cout << "[Server] Waiting for connections on " << server_port << "...\n";

		if((childfd = accept(sockfd, (struct sockaddr *) &cli_addr, &cli_len)) < 0) {
			cout << "[Server] Failed to accept\n";
			continue;
		}
        	char * line =NULL;
        	size_t len =0;
		FILE* fp = fdopen(childfd, "r");
		string temp_resquest_quee[1];
		int idx=1;
		int get_header=0;
		string url ;
		string responseHeader;
        	while(getline(&line,&len,fp)){
			string x(line);
			//handle Header
			if(idx==1){
				temp_resquest_quee[0]=x;
				vector<string> firstline=split(x,' ');
				url ="."+firstline[1];
				printf("%s\n",url.c_str());
				get_header=1;	
			}

			idx+=1;
			//Response
			if(x=="\r\n"&&get_header==1){
				printf("sentresponse");
				responseHeader="HTTP/1.1 200 OK\r\n";
				write(childfd,responseHeader.c_str(),strlen(responseHeader.c_str()));
				responseHeader="Content-Type: text/html\r\n";
				write(childfd,responseHeader.c_str(),strlen(responseHeader.c_str()));
				responseHeader="\r\n";
				write(childfd,responseHeader.c_str(),strlen(responseHeader.c_str()));
				//Response Date
				printf("============%s==============\n",url.c_str());
				FILE* Reshtml = fopen(url.c_str(),"r");
				if(Reshtml){
					while(getline(&line,&len,Reshtml)!=EOF){
						write(childfd,line,strlen(line));
					}
				}else{
					responseHeader="404";
					write(childfd,responseHeader.c_str(),strlen(responseHeader.c_str()));
					responseHeader="\r\n";
					write(childfd,responseHeader.c_str(),strlen(responseHeader.c_str()));
				}
				responseHeader="\r\n";
				write(childfd,responseHeader.c_str(),strlen(responseHeader.c_str()));
				/*
				responseHeader="It's Works";
				write(childfd,responseHeader.c_str(),strlen(responseHeader.c_str()));
				responseHeader="\r\n";
				write(childfd,responseHeader.c_str(),strlen(responseHeader.c_str()));*/
				break;
			}
			x="server response : "+x;
			
			printf("%s",x.c_str());
			
			//write(childfd,x.c_str(),strlen(x.c_str()));
		};		
		
		// Fork child socket
		if(_DEBUG)	cout << "[Server] Connected\n";
		close(childfd);
	}

	return 0;
}
