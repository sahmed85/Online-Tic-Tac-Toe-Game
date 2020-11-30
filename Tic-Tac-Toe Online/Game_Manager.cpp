#include "Game_Manager.h"
#include <iostream>
#include <thread>
#include <list>
#include <string>
#include <mutex>
#include <cstring>
#include <array>
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

void recv_SocketMsgs(Game_Manager* pUdpSocket) {
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
		n = recvfrom(pUdpSocket->sockfd, (char*)&recv_message, sizeof(recv_message), 0, (struct sockaddr*)&from, &fromlen);
		if (n < 0) {
			//if n<0 then there is an error on the socket
			//useful for when we call the close on socket, the code will be able to break out of this infinite loop
			break;
		}
		else {
			//process messages here
			//only process messages with the version 1
			if (recv_message.nVersion == 1) {
				//handle different message types, etc.
				if (recv_message.messageType == 1) {
					//check the sender of this message
					if ((pUdpSocket->player_1.sin_addr.S_un.S_addr == from.sin_addr.S_un.S_addr) && (pUdpSocket->player_1.sin_port == from.sin_port)) {
						//then we know that this is the 'X' move
						//double check here with the message contents
						if (recv_message.message == "X") {
							cout << "Checked" << endl;
						}
						//update the grid and call check_winner
						pUdpSocket->game_grid[recv_message.command_pos] = 'X';
						//send other player update board packet
						udpMessage update_pkt1;
						update_pkt1.messageType = 4;
						update_pkt1.command_pos = recv_message.command_pos;
						update_pkt1.message = "X";
						update_pkt1.messageLength = sizeof(update_pkt1.message);

						n = sendto(pUdpSocket->sockfd, (char*)&update_pkt1, sizeof(udpMessage), 0, (struct sockaddr*)&pUdpSocket->player_2, sizeof(sockaddr_in));

						//make sure no errors
						if (n < 0) {
							pUdpSocket->error("ERROR sending board update packet to Player2");
						}
					}
					else if ((pUdpSocket->player_2.sin_addr.S_un.S_addr == from.sin_addr.S_un.S_addr) && (pUdpSocket->player_2.sin_port == from.sin_port)) {
						//then we know that this is the 'O' move
						//double check here with the message contents
						if (recv_message.message == "O") {
							cout << "Checked" << endl;
						}
						//update the grid and call check_winner
						pUdpSocket->game_grid[recv_message.command_pos] = 'O';
						//send other player update board packet
						udpMessage update_pkt2;
						update_pkt2.messageType = 4;
						update_pkt2.command_pos = recv_message.command_pos;
						update_pkt2.message = "O";
						update_pkt2.messageLength = sizeof(update_pkt2.message);

						n = sendto(pUdpSocket->sockfd, (char*)&update_pkt2, sizeof(udpMessage), 0, (struct sockaddr*)&pUdpSocket->player_1, sizeof(sockaddr_in));

						//make sure no errors
						if (n < 0) {
							pUdpSocket->error("ERROR sending board update packet to Player1");
						}

					}
					//check if there is an winner and send game won/lost packets
					//call bool to check if grid is won by any players
					int player_won = pUdpSocket->check_winner();
					if (player_won == 3 || player_won == 1 || player_won == 2) {
						//if player_won = 3; tie
						if (player_won == 3) {
							udpMessage tied;
							tied.messageType = 7;
							tied.command_pos = -1;
							tied.message = "Tied";
							tied.messageLength = sizeof(tied.message);

							n = sendto(pUdpSocket->sockfd, (char*)&tied, sizeof(udpMessage), 0, (struct sockaddr*)&pUdpSocket->player_1, sizeof(sockaddr_in));

							if (n < 0) {
								pUdpSocket->error("ERROR sending tied packet to Player1");
							}

							n = sendto(pUdpSocket->sockfd, (char*)&tied, sizeof(udpMessage), 0, (struct sockaddr*)&pUdpSocket->player_2, sizeof(sockaddr_in));

							if (n < 0) {
								pUdpSocket->error("ERROR sending tied packet to Player2");
							}
							pUdpSocket->game_done = true;
						}
						else {
							//if 1, then player1 one or if 2 player2 
							udpMessage game_won;
							game_won.messageType = 5;
							game_won.command_pos = -1;
							game_won.message = "Winner";
							game_won.messageLength = sizeof(game_won.message);

							udpMessage game_lost;
							game_lost.messageType = 6;
							game_lost.command_pos = -1;
							game_lost.message = "Loser";
							game_lost.messageLength = sizeof(game_lost.message);
							if (player_won == 1) {
								//send game won packet to player one and send game lost packet to player two
								n = sendto(pUdpSocket->sockfd, (char*)&game_won, sizeof(udpMessage), 0, (struct sockaddr*)&pUdpSocket->player_1, sizeof(sockaddr_in));

								if (n < 0) {
									pUdpSocket->error("ERROR sending game won packet to Player1");
								}

								n = sendto(pUdpSocket->sockfd, (char*)&game_lost, sizeof(udpMessage), 0, (struct sockaddr*)&pUdpSocket->player_2, sizeof(sockaddr_in));

								if (n < 0) {
									pUdpSocket->error("ERROR sending game won packet to Player2");
								}
								pUdpSocket->game_done = true;
							}
							else if (player_won == 2) {
								//send game won packet to player two and send game lost packet to player one
								n = sendto(pUdpSocket->sockfd, (char*)&game_won, sizeof(udpMessage), 0, (struct sockaddr*)&pUdpSocket->player_2, sizeof(sockaddr_in));

								if (n < 0) {
									pUdpSocket->error("ERROR sending game won packet to Player2");
								}

								n = sendto(pUdpSocket->sockfd, (char*)&game_lost, sizeof(udpMessage), 0, (struct sockaddr*)&pUdpSocket->player_1, sizeof(sockaddr_in));

								if (n < 0) {
									pUdpSocket->error("ERROR sending game won packet to Player1");
								}
								pUdpSocket->game_done = true;
							}
						}
					}
				}
			}
		}
	} while (true);
}


Game_Manager::Game_Manager(sockaddr_in player1, sockaddr_in player2)
{
	player_1 = player1;
	player_2 = player2;

	//init the socket that GameManager will manipulate
	//setup socket
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
	serv_addr.sin_port = htons(0); //in this case, we will let OS pick the portnumber to communicate with this thread; client will know to communicate with it
	
	//attempt to bind socket to port number
	if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
		error("ERROR in binding socket to port number");
	}
	cout << "Socket created" << endl;

	//start a thread on function receiveSocketMsgs, that will listen for packets and resolve them appropiately
	//this thread has a pointer to this object to access members
	m_recvThread = thread(recv_SocketMsgs, this);
	//send game init packet to player 1 and 2
	int n;
	//construct the message; lets do player1 first
	udpMessage game_init;
	game_init.messageType = 3;
	game_init.command_pos = -1;
	game_init.message = "Player1";
	game_init.messageLength = sizeof(game_init.message);

	n = sendto(sockfd, (char*)&game_init, sizeof(udpMessage), 0, (struct sockaddr*)&player_1, sizeof(sockaddr_in));

	//make sure no errors
	if (n < 0) {
		error("ERROR sending game_init packet to Player1");
	}

	//construct the message for player2
	game_init.message.clear();
	game_init.message = "Player2";

	n = sendto(sockfd, (char*)&game_init, sizeof(udpMessage), 0, (struct sockaddr*)&player_2, sizeof(sockaddr_in));

	if (n < 0) {
		error("ERROR sending game_init packet to Player2");
	}
}

Game_Manager::~Game_Manager() {
	//close the socket first, this will trigger the break in the recv thread and will allow it to gracefully join
	//close the socket
	sockClose(sockfd);
	sockQuit();

	//socket recv from should error out (n<0) and break out of loop
	//join that thread back with main
	m_recvThread.join();

	//terminate the program with status 0 -- graceful exit 
	//exit(0);

}


int Game_Manager::check_winner() {
	////check grid if we have a winner or tie

	//check horizontals
	for (int i = 0; i <= 2; ++i) {
		if ((game_grid[(i * 3)] == 'X' || game_grid[(i * 3)] == 'O') && (game_grid[(i * 3)] == game_grid[(i * 3) + 1]) && (game_grid[(i * 3)] == game_grid[(i * 3) + 2])) {
			if (game_grid[(i * 3)] == 'X') {
				//game_done = true;
				cout << "Horizontal found." << endl;
				return 1;
			}
			else {
				//game_done = true;
				cout << "Horizontal found." << endl;
				return 2;
			}
		}
	}

	//check vertical
	for (int j = 0; j <= 2; ++j) {
		if ((game_grid[(j)] == 'X' || game_grid[(j)] == 'O') && (game_grid[j] == game_grid[j + 3]) && (game_grid[j] == game_grid[j + 6])) {
			if (game_grid[j] == 'X') {
				//game_done = true;
				return 1;
			}
			else {
				//game_done = true;
				return 2;
			}
		}
	}

	//check diagnol
	if ((game_grid[(2)] == 'X' || game_grid[(2)] == 'O') && (game_grid[2] == game_grid[4]) && (game_grid[2] == game_grid[6])) {
		if (game_grid[2] == 'X') {
			//game_done = true;
			return 1;
		}
		else {
			//game_done = true;
			return 2;
		}
	}
	else if ((game_grid[(0)] == 'X' || game_grid[(0)] == 'O') && (game_grid[0] == game_grid[4]) && (game_grid[0] == game_grid[8])) {
		if (game_grid[0] == 'X') {
			//game_done = true;
			return 1;
		}
		else {
			//game_done = true;
			return 2;
		}
	}
	//game_done = true;

	//check if all grid spots filled
	for (int k = 0; k < 9; ++k) {
		if (game_grid[k] == '1') {
			return 0;
		}
	}
	//if none of the above, return tied
	return 3;
}


//socket init and quit function as well as error handling -- Cross Platform implementation
int Game_Manager::sockInit(void) {
#ifdef _WIN32
	WSADATA wsa_data;
	return WSAStartup(MAKEWORD(1, 1), &wsa_data);
#else
	return 0;
#endif
}

int Game_Manager::sockQuit(void) {
#ifdef _WIN32
	return WSACleanup();
#else
	return 0;
#endif
}

int Game_Manager::sockClose(SOCKET sock) {
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

void Game_Manager::error(const char* msg) {
	perror(msg);
	// Make sure any open sockets are closed before calling exit
	//exit with status code 1
	exit(1);
}