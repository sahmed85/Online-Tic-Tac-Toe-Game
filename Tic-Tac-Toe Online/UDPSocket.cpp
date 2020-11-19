#include "UDPSocket.h"
#include <iostream>
#include <thread>
#include <list>
#include <string>
#include <mutex>
#include <cstring>

using namespace std;

//this function will called by the class to listen on its socket for incoming messages
//this function will handle the message itself and updat the class as needed
void receiveSocketMessage(UDPSocket* udp_socket) {

}

UDPSocket::UDPSocket(unsigned short port_num) :udp_Port(port_num) {
	//setup socket
}

UDPSocket::~UDPSocket() {
	//custom destructor
}

//void UDPSocket::sendPacket(const sockaddr_in client, unsigned short port_num, const udpMessage& msg) {
//	//this function sends a UDP message to target (client in this case)
//}


//cross platform socket init
int UDPSocket::sockInit(void) {

}

int UDPSocket::sockQuit(void) {

}

int UDPSocket::sockClose(SOCKET sock) {

}

void UDPSocket::error(const char* msg) {

}