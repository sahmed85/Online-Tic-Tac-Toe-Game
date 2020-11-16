#include <SFML/Graphics.hpp>
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
    
    //create the client class and attempt to connect to server (use socket programming), server is a set address
    //send message type for init
    //display to console, joining matchmaking, waiting for other player  

    //
    
    return 0;
}