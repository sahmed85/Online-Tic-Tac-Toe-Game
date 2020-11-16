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

	//create the server socket class
	
	//do..while listen for packets and resolve them
	//if message type game init, then begin game with next init that is sent
	
}
