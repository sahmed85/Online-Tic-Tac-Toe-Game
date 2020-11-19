#include "Game_Manager.h"
#include <iostream>
#include <thread>
#include <list>
#include <string>
#include <mutex>
#include <cstring>

using namespace std;

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

Game_Manager::Game_Manager(sockaddr_in player1, sockaddr_in player2)
	//:player_1(player1), player_2(player2)
{
	//custom constructor 
}

Game_Manager::~Game_Manager() {
	//custom destructor
}

void Game_Manager::sendMessage(const sockaddr_in client, unsigned short port_num, const udpMessage& msg) {
	//send udpMessage to client on game move, winner or game beginning 
}

bool Game_Manager::check_winner() {
	//check grid if we have a winner
}


//socket init and quit function as well as error handling -- Cross Platform implementation
int Game_Manager::sockInit(void) {

}

int Game_Manager::sockQuit(void) {

}

int Game_Manager::sockClose(SOCKET sock) {

}

void Game_Manager::error(const char* msg) {

}