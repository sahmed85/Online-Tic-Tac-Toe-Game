#pragma once

//implement the header for Game_Manager here
//this is the class that will handle the game between two players

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

struct udpMessage {
    unsigned short nVersion;
    unsigned short messageType;
    unsigned short messageLength;
    unsigned long command_seq;
    char message[100];
};


/////////////////



//bool UDPSocket::check_winner() {
//	//check if any winners in game_grid
//}
/////////////////////////



class Game_Manager {
public:
    Game_Manager() = delete;

    Game_Manager(sockaddr_in player1, sockaddr_in player2);

    ~Game_Manager();

    void sendMessage(const sockaddr_in client, unsigned short port_num, const udpMessage& msg);

    bool check_winner();

    //member functions that will help init and close sockets
    int sockInit(void);

    int sockQuit(void);

    int sockClose(SOCKET sock);

    void error(const char* msg);

    //public member variables

private:
    std::thread m_recvThread;
    sockaddr_in player_1;
    sockaddr_in player_2;
    char game_grid[9] = {'0'};

};
