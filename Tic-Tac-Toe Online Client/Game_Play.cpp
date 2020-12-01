//Author: Shadman Ahmed
//Class: ECE 4122
//Assingment: Lab5
//Date: 11/17/2020
//Last Modified:12/01/2020 
//Overview: This file contains the implementation for the Game_Play class that will manage game control and graphics for the player.
//Github/Git: https://github.com/sahmed85/Online-Tic-Tac-Toe-Game

#include <iostream>
#include <thread>
#include <list>
#include <string>
#include <mutex>
#include <cstring>
#include "Game_Play.h"

using namespace std;

#pragma warning( disable : 4996)

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

//this function will in a seperate thread that will handle message recevied by the server 
void recv_SocketMsgs(Game_Play* game) {
	int n;
	//hold received message in this
	udpMessage recv_message;
	//hold the IP and port info of the sender here
	struct sockaddr_in from;
	socklen_t fromlen = sizeof(struct sockaddr_in);
	//Infinite loop unless socket errors out, then break and end scope. This will allow the thread to join back with the object that called it. 
	do {
		//reset the value of recv_message to 0 since last received message
		recv_message.message.clear();
		//blocking call -- recvfrom(...); allows us to read in the message, it fills in the memory buffer of the sizeof(udpMessage)
		n = recvfrom(game->sockfd, (char*)&recv_message, sizeof(recv_message), 0, (struct sockaddr*)&from, &fromlen);
		if (n < 0) {
			//if n<0 then there is an error on the socket
			//useful for when we call the close on socket, the code will be able to break out of this infinite loop
			break;
		}
		else {
			//process messages here
			//only process messages with version 1 
			if (recv_message.nVersion == 1) {
				//process messages sent to client here from server
				if (recv_message.messageType == 3) {
					//received game init packet; save the servers information
					//set connected flag to true
					//set the move type
					game->server = from;
					game->connected = true;
					cout << "Set Server IP" << endl;
					if (recv_message.message == "Player1") {
						game->player = 'X';
						game->canMove = true;
						game->displayText = "Your move.";
					}
					else {
						game->player = 'O';
						game->canMove = false;
						game->displayText = "Opponents move.";
					}
				}
				else if (recv_message.messageType == 4) {
					//received opponents move from server
					//update the game_grid and the disable movement flag
					cout << "Received Move" << endl;
					char move = recv_message.message[0];
					game->updateGrid(recv_message.command_pos, move);
					game->canMove = true;
					game->displayText = "Your move.";
				}
				else if (recv_message.messageType == 5) {
					//disable movement
					game->canMove = false;
					//game won!
					game->displayText = "You won!\tPress esc to exit game.";
					game->game_done = true;
				}
				else if (recv_message.messageType == 6) {
					//disable movement
					game->canMove = false;
					//game lost
					game->displayText = "You lost!\tPress esc to exit game.";
					game->game_done = true;
					
				}
				else if (recv_message.messageType == 7) {
					//disable movement
					game->canMove = false;
					//game tied
					game->displayText = "Tied!\tPress esc to exit game.";
					game->game_done = true;
				}
			}
		}
	} while (true);
}


Game_Play::Game_Play() {
	//init the socket that GameManager will manipulate
	//setup socket
	sockaddr_in player_addr;
	sockInit();

	//create the socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0); //where AF_INET is IPv4, SOCK_DGRAM is UDP and protocol = 0 is IP
	//check if error; sockfd < 0
	if (sockfd < 0) {
		error("ERROR in Opening Socket");
	}

	//set the struct values of serv_addr to bind the socket to these IP address properties
	player_addr.sin_family = AF_INET; //family is AF_INET = IPv4
	player_addr.sin_addr.s_addr = INADDR_ANY; //bind socket to all IP of this machine, usually one for this test purpose
	// Convert port number from host to network
	player_addr.sin_port = htons(0); //in this case, we will let OS pick the portnumber to communicate with this thread; client will know to communicate with it

	//attempt to bind socket to port number
	if (bind(sockfd, (struct sockaddr*)&player_addr, sizeof(player_addr)) < 0) {
		error("ERROR in binding socket to port number");
	}
	cout << "Socket created" << endl;
	//start a thread on function receiveSocketMsgs, that will listen for packets and resolve them appropiately
	//this thread has a pointer to this object to access members
	m_recvThread = thread(recv_SocketMsgs, this);
	
	//make to send message to server
	string server_ip = "localhost";
	struct hostent* server;
	struct sockaddr_in serv_addr;
	int n;
	//convert localhost to hostent 
	server = gethostbyname(server_ip.c_str());
	// Zero out serv_addr variable
	memset((char*)&serv_addr, sizeof(serv_addr), 0);
	serv_addr.sin_family = AF_INET;
	memmove((char*)&serv_addr.sin_addr.s_addr, (char*)server->h_addr, server->h_length);
	//this port is listening for matchmaking
	serv_addr.sin_port = htons(6100);

	//construct the matchmaking packer
	udpMessage machmaking_init;
	machmaking_init.messageType = 0;
	machmaking_init.command_pos = -1;
	machmaking_init.message = "NA";
	machmaking_init.messageLength = sizeof(machmaking_init.message);

	n = sendto(sockfd, (char*)&machmaking_init, sizeof(udpMessage), 0, (struct sockaddr*)&serv_addr, sizeof(sockaddr_in));

	//make sure no errors
	if (n < 0) {
		error("ERROR sending machmaking_init packet to Player1");
	}
	cout << "Sent Matchmaking Packet to server" << endl;
}

//destructor needs to handle closing the socket and join the recv thread 
Game_Play::~Game_Play() {
	//close the socket first, this will trigger the break in the recv thread and will allow it to gracefully join
	//close the socket
	sockClose(sockfd);
	sockQuit();

	//socket recv from should error out (n<0) and break out of loop
	//join that thread back with main
	m_recvThread.join();

	//terminate the program with status 0 -- graceful exit 
	exit(0);
}

//helper function that update member variable (grid)
void Game_Play::updateGrid(int pos, char mv_type) {
	game_grid[pos] = mv_type;
}

//helper function that will use the socket to send a message to the server
void Game_Play::sendMove(int pos) {
	//send move to server, messageType = 1, message is game->player.
	cout << "Trying to send!" << endl;
	int n;
	udpMessage send_move;
	send_move.messageType = 1;
	send_move.command_pos = pos;
	send_move.message = player;
	send_move.messageLength = sizeof(send_move.message);
	
	//send packet
	n = sendto(sockfd, (char*)&send_move, sizeof(udpMessage), 0, (struct sockaddr*)&server, sizeof(server));
	
	if (n < 0) {
		error("ERROR sending move packet to server");
	}

	cout << "Sent" << endl;
}

//socket init and quit function as well as error handling -- Cross Platform implementation
int Game_Play::sockInit(void) {
#ifdef _WIN32
	WSADATA wsa_data;
	return WSAStartup(MAKEWORD(1, 1), &wsa_data);
#else
	return 0;
#endif
}

int Game_Play::sockQuit(void) {
#ifdef _WIN32
	return WSACleanup();
#else
	return 0;
#endif
}

int Game_Play::sockClose(SOCKET sock) {
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

void Game_Play::error(const char* msg) {
	perror(msg);
	// Make sure any open sockets are closed before calling exit
	//exit with status code 1
	exit(1);
}