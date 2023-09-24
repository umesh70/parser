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

struct thread_data {
	SOCKET socfd;
	SOCKET newfd;
	std::string data;
};

int main() {

}