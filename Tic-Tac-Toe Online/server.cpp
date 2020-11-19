#include <iostream>
#include <sstream>
#include <stdlib.h>
//add include for class header

#pragma warning(disable : 4996)

#ifdef _WIN32
/* See http://stackoverflow.com/questions/12765743/getaddrinfo-on-win32 */
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501  /* Windows XP. */
#endif
#include <winsock2.h>
#include <Ws2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")
#else
/* Assume that any non-Windows platform uses POSIX-style sockets instead. */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>  /* Needed for getaddrinfo() and freeaddrinfo() */
#include <unistd.h> /* Needed for close() */

typedef int SOCKET;
#endif

using namespace std;

int main() {
	//no command line args to resolve

	//init the socket for server to listen on -- create a UDPSocket object and pass in the port 
	//server will listen on port 6100 for new players
	
	//infinite loop for either quit command or 
	//see if sources > 2, if so, then take the first two of the vector 
	//and call a thread to start a Game_Manager session for them
	
}
