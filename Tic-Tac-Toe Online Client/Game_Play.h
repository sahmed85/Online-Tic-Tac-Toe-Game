//Author: Shadman Ahmed
//Class: ECE 4122
//Assingment: Lab5
//Date: 11/17/2020
//Last Modified:12/01/2020 
//Overview: This file contains the header for the class that will manage game control and graphics for the player.
//Github/Git: https://github.com/sahmed85/Online-Tic-Tac-Toe-Game

#pragma once

#include <iostream>
#include <thread>
#include <list>
#include <string>
#include <mutex>
#include <cstring>
#include <array>
//SFML Library
#include <SFML/Graphics.hpp>

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
    unsigned short nVersion = 1;
    unsigned short messageType;
    unsigned short messageLength;
    unsigned int command_pos;
    std::string message;
};


class Game_Play {
public:
    Game_Play();

    ~Game_Play();

    void updateGrid(int pos, char mvType);

    void sendMove(int pos);

    //member functions that will help init and close sockets
    int sockInit(void);

    int sockQuit(void);

    int sockClose(SOCKET sock);

    void error(const char* msg);

    //public member variables
    int sockfd;
    std::array<char, 9 > game_grid = {'1', '1', '1', '1', '1', '1', '1', '1', '1'};
    sockaddr_in server;
    bool canMove;
    char player;
    bool connected = false;
    std::string displayText = "";
    bool game_done = false;

private:
    std::thread m_recvThread;

};