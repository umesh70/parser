#include<winsock2.h>  //newer version of windows socket library
#include<ws2tcpip.h>
#include<sys/types.h>
#include<stdint.h>
#include<signal.h>
#include<sys/types.h>
#include<string>
#include<stdlib.h>
#include<errhandlingapi.h>

#include "inc/clientparser.h"

#define PORT "3000"
#define THREAD_NUM 5
#define MaxDataSize 4096

struct thread_data {
	SOCKET newfd;
	std::string data;
	SERVER* server;
};

/*class thread {
	SOCKET socfd;
	std::string data;
};*/


class SERVER {

	//SOCKET members
	WSADATA wsaData;		// for initiating the windows socket
	struct addrinfo hints,				//our preferences 
		* server_info,		//will point to the linked list of available address
		* p;					//pointer to the single connection
	struct sockaddr_storage their_addr;  // to store socket address info
	int sockfd;					//to listen for incoming connections  
	int con_fd[THREAD_NUM];		//to setup a connection btw the server and a node 	
	socklen_t sin_size;
	char address[INET6_ADDRSTRLEN];
	int check_addrinfo;
	int check_recv;
	int yes = 1;
	thread_data datarray[1];
	char data[MaxDataSize];


public:
	SERVER() {
		//initiating the windows socket
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
			fprintf(stderr, "WSAStartup failed.\n");
			exit(1);
		}

		if (LOBYTE(wsaData.wVersion) != 2 ||
			HIBYTE(wsaData.wVersion) != 2) {
			fprintf(stderr, "Versiion 2.2 of Winsock is not available.\n");
			WSACleanup();
			exit(2);
		}
		//specifying the preferences for our connection
		memset(&hints, 0, sizeof hints);
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;
	}

	void setup_socket() {

		if ((check_addrinfo = getaddrinfo(NULL, PORT, &hints, &server_info)) != 0) {
			fprintf(stderr, "getaddrinfo:%s\n", gai_strerror(check_addrinfo));
			return;
		}
		//to get the available address
		for (p = server_info; p != NULL; p = p->ai_next) {
			if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
				perror("server: socket");
				return;
			}

			if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&yes, sizeof(int)) == -1) {
				perror("setsockopt");
				exit(1);
			}

			if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
				closesocket(sockfd);
				perror("server: bind");
				continue;
			}
		}

		freeaddrinfo(server_info);

		if (p == NULL) {
			fprintf(stderr, "server: failed to bind\n");
			exit(1);
		}

		if (listen(sockfd, SOMAXCONN) == -1) {
			perror("Listen");
			exit(1);
		}

		printf("server: waiting for connection...\n");

	}

	void accept_connection() {

		int connections_num = 0;
		while (connections_num != THREAD_NUM) {
			
			sin_size = sizeof their_addr;
			con_fd[connections_num] = accept(sockfd, (struct sockaddr*)&their_addr, &sin_size);
			if (con_fd[connections_num] == -1) {
				perror("accept");
				continue;
			}
			else {
				//datarray->newfd = con_fd[connections_num];
				HANDLE cthread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) threadfunction, (LPVOID*)con_fd[connections_num], 0, NULL);
				if (cthread == NULL)
					perror("create thread");
				else
					connections_num++;
			}
		}
	}

	void* get_in_addr(struct sockaddr* sa) {

		if (sa->sa_family == AF_INET) {
			return &(((struct sockaddr_in*)sa)->sin_addr);
		}

		return &(((struct sockaddr_in6*)sa)->sin6_addr);
	}

	DWORD threadfunction(LPVOID thparameters) {
		
		inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr*)&their_addr),address , sizeof address);
		printf("server got connection from : %s\n", address);
		printf("recieving data...");
		int fd = (int)thparameters;
		if (check_recv = recv(fd, data, MaxDataSize - 1, 0) == -1)
			perror("recv");
		data[check_recv] = '\0';
		printf("command recieved ");
	}

	 static DWORD WINAPI threadfunction(LPVOID thparameters) {
		 SERVER* instance = static_cast<SERVER*>(thparameters);
		 return instance->threadfunction(thparameters);
	}

	
};



int main() {

}
