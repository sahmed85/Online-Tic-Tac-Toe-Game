//Author: Shadman Ahmed
//Class: ECE 4122
//Assingment: Lab5
//Date: 11/17/2020
//Last Modified:12/01/2020 
//Overview: This file contains the main execution of the server side code. This code handles matchmaking and spawns threads to manage gameplay between players.
//Github/Git: https://github.com/sahmed85/Online-Tic-Tac-Toe-Game

#include <iostream>
#include <thread>
#include <list>
#include <string>
#include <mutex>
#include <cstring>
#include "Game_Manager.h"
#include <array>
#include <chrono> 

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

void create_game(sockaddr_in p1, sockaddr_in p2);
int sockInit(void);
int sockQuit(void);
int sockClose(SOCKET sock);
void error(const char* msg);

//these list will hold the Network info of players that entered matchmaking
list<sockaddr_in> match_queue;


int main() {
	//server will listen on port 6100 for new players and handle matchmaking init packets

	//init the socket that GameManager will manipulate
	//setup socket
	int sockfd;
	sockaddr_in serv_addr;
	sockInit();

	//create the socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0); //where AF_INET is IPv4, SOCK_DGRAM is UDP and protocol = 0 is IP
	//check if error; sockfd < 0
	if (sockfd < 0) {
		error("ERROR in Opening Socket");
	}

	//set the struct values of serv_addr to bind the socket to these IP address properties
	serv_addr.sin_family = AF_INET; //family is AF_INET = IPv4
	serv_addr.sin_addr.s_addr = INADDR_ANY; //bind socket to all IP of this machine, usually one for this test purpose
	// Convert port number from host to network
	serv_addr.sin_port = htons(6100); //in this case, we will let OS pick the portnumber to communicate with this thread; client will know to communicate with it

	//attempt to bind socket to port number
	if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
		error("ERROR in binding socket to port number");
	}
	cout << "Socket created (server.cpp)" << endl;
	//infinite loop for either quit command or 
	//see if sources > 2, if so, then take the first two of the vector 
	//and call a thread to start a Game_Manager session for them
	
	int n;
	//hold received message here
	udpMessage recv_message;
	//hold IP and port info of client here
	struct sockaddr_in from;
	socklen_t fromlen = sizeof(struct sockaddr_in);

	do {
		recv_message.message.clear();
		//blocking call -- recvfrom(...); allows us to read in message, fills memory buffer
		n = recvfrom(sockfd, (char*)&recv_message, sizeof(recv_message), 0, (struct sockaddr*)&from, &fromlen);
		if (n < 0) {
			//if n<0 then there is an error on the socket
			break;
		}
		else {
			if (recv_message.nVersion == 1 && recv_message.messageType == 0) {
				//received a matchmaking packet, push from info to list and check if list >=2
				match_queue.push_back(from);
				cout << "Received Matchmaking Request" << endl;
				if (match_queue.size() == 2) {
					thread th(create_game, match_queue.front(), match_queue.back());
					//detach the thread, since it has no dependency with main and can finish execution
					//without affecting main
					th.detach();
					match_queue.pop_back();
					match_queue.pop_back();
				}
			}
		}
	} while (true);


	sockClose(sockfd);
	sockQuit();
}

//spawn this thread that will create a GameManager thread for the players
//this thread should be deattached from the server.cpp thread and run independently 
void create_game(sockaddr_in p1, sockaddr_in p2) {
	//create a game_manager object and let it handle game communications.
	Game_Manager game(p1, p2);
	cout << "In Game Thread" << endl;
	while (!game.game_done) {

	}
	std::this_thread::sleep_for(std::chrono::seconds(5));
	cout << "Game Complete, exiting thread." << endl;
	//game.~Game_Manager();
	
}

//socket init and quit function as well as error handling -- Cross Platform implementation
int sockInit(void) {
#ifdef _WIN32
	WSADATA wsa_data;
	return WSAStartup(MAKEWORD(1, 1), &wsa_data);
#else
	return 0;
#endif
}

int sockQuit(void) {
#ifdef _WIN32
	return WSACleanup();
#else
	return 0;
#endif
}

int sockClose(SOCKET sock) {
	int status = 0;

#ifdef _WIN32
	status = shutdown(sock, SD_BOTH);
	if (status == 0)
	{
		status = closesocket(sock);
	}
#else
	status = shutdown(sock, SHUT_RDWR);
	if (status == 0)
	{
		status = close(sock);
	}
#endif

	return status;
}

void error(const char* msg) {
	perror(msg);
	// Make sure any open sockets are closed before calling exit
	//exit with status code 1
	exit(1);
}

