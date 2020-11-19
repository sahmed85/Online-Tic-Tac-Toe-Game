#pragma once

//implement header for UDPSocket class
//UDP socket class is general purpose UDPsocket creator
//All devices will communicate over a custom structure

#include <iostream>
#include <thread>
#include <list>
#include <string>
#include <mutex>
#include <cstring>


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

//this is the inital draft of the udpMessage structure that will be passed around by clients and and servers
struct udpMessage {
    unsigned short nVersion;
    unsigned short messageType;
    unsigned short messageLength;
    unsigned long command_seq;
    char message[100];
};

class UDPSocket
{
public:
    UDPSocket() = delete;

    UDPSocket(unsigned short port_num);

    ~UDPSocket();

    //void sendPacket(const sockaddr_in client, unsigned short port_num, const udpMessage& msg);


   //socket functions for UDP
    int sockInit(void);

    int sockQuit(void);

    int sockClose(SOCKET sock);

    void error(const char* msg);

    //public variables
    std::list<sockaddr_in> m_lstSources;
private:
    unsigned short udp_Port;
    std::thread recv_thread;
    //mutex?
   
};