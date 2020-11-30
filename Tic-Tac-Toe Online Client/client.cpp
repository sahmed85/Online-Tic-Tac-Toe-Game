#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <thread>
#include <list>
#include <string>
#include <array>
#include <cstring>
#include "Game_Play.h"
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

void drawXO(int grid_pos, char shape_type, sf::RenderWindow& this_window);

string you_are = "";

int main() {
    //no command line args to resolve
    
    //create a game_play object, wait till connected flag is set
    Game_Play game;
    cout << "Entered matchmaking. Please wait ..." << endl;

    while (!game.connected) {
        
    }

    cout << "Connected!" << endl;

    //declare rendering window and font
    sf::RenderWindow window(sf::VideoMode(900, 900), "Tic-Tac-Toe Online");
    sf::Font font_bold;
    if (!font_bold.loadFromFile("Cabin-Bold.ttf")) {
        exit(1);
    }

    //SFML event handler
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                //only process mouse buttons if canMove flag is set
                cout << "Got here atleast" << endl;
                cout << "Move Flag is " << game.canMove << endl;
                if (game.canMove) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        //check where in the window it was pressed 
                        if (event.mouseButton.x < 290 && event.mouseButton.y < 290 && game.game_grid[0] == '1') {
                            //grid pos 0
                            game.canMove = false;
                            game.game_grid[0] = game.player;
                            //maybe update text here?
                            //send move to server
                            game.sendMove(0);
                        }
                        else if ((event.mouseButton.x > 310 && event.mouseButton.x < 590) && (event.mouseButton.y < 290) && game.game_grid[1] == '1') {
                            //grid pos 1
                            cout << "CLICK HERE" << endl;
                            game.canMove = false;
                            game.game_grid[1] = game.player;
                            game.sendMove(1);
                            game.displayText = "Opponents move.";
                        }
                        else if ((event.mouseButton.x > 610) && (event.mouseButton.y < 290) && game.game_grid[2] == '1' ) {
                            //grid pos 2
                            game.canMove = false;
                            game.game_grid[2] = game.player;
                            game.sendMove(2);
                            game.displayText = "Opponents move.";
                        }
                        else if ((event.mouseButton.x < 290) && (event.mouseButton.y > 310 && event.mouseButton.y < 490) && game.game_grid[3] == '1') {
                            //grid pos 3
                            game.canMove = false;
                            game.game_grid[3] = game.player;
                            game.sendMove(3);
                            game.displayText = "Opponents move.";
                        }
                        else if ((event.mouseButton.x > 310 && event.mouseButton.x < 590) && (event.mouseButton.y > 310 && event.mouseButton.y < 490) && game.game_grid[4] == '1') {
                            //grid pos 4
                            game.canMove = false;
                            game.game_grid[4] = game.player;
                            game.sendMove(4);
                            game.displayText = "Opponents move.";
                        }
                        else if ((event.mouseButton.x > 610) && (event.mouseButton.y > 310 && event.mouseButton.y < 490) && game.game_grid[5] == '1') {
                            //grid pos 5
                            game.canMove = false;
                            game.game_grid[5] = game.player;
                            game.sendMove(5);
                            game.displayText = "Opponents move.";
                        }
                        else if ((event.mouseButton.x < 290) && (event.mouseButton.y > 510 && event.mouseButton.y < 690) && game.game_grid[6] == '1') {
                            //grid pos 6
                            game.canMove = false;
                            game.game_grid[6] = game.player;
                            game.sendMove(6);
                            game.displayText = "Opponents move.";
                        }
                        else if ((event.mouseButton.x > 310 && event.mouseButton.x < 590) && (event.mouseButton.y > 510 && event.mouseButton.y < 690) && game.game_grid[7] == '1') {
                            //grid pos 7
                            game.canMove = true;
                            game.game_grid[7] = game.player;
                            game.sendMove(7);
                            game.displayText = "Opponents move.";
                        }
                        else if ((event.mouseButton.x > 610) && (event.mouseButton.y > 510 && event.mouseButton.y < 690) && game.game_grid[8] == '1') {
                            //grid pos 8
                            game.canMove = false;
                            game.game_grid[8] = game.player;
                            game.sendMove(8);
                            game.displayText = "Opponents move.";
                        }
                    }
                }
            }
            else if (sf::Event::KeyPressed) {
                if (game.game_done) {
                    if (event.key.code == sf::Keyboard::Escape)
                    {
                        //call the destructor and exit gracefully
                        game.~Game_Play();
                    }
                }
            }
        }
        /*
        Calling clear before drawing anything is mandatory, otherwise the contents from previous frames will be present behind anything you draw.
        The only exception is when you cover the entire window with what you draw, so that no pixel is not drawn to.
        In this case you can avoid calling clear (although it won't have a noticeable impact on performance).

        Calling display is also mandatory, it takes what was drawn since the last call to display and displays it on the window.
        Indeed, things are not drawn directly to the window, but to a hidden buffer.
        This buffer is then copied to the window when you call display -- this is called double-buffering.
        */

        //clear the window with black color
        window.clear(sf::Color::Black);

        // draw TTT grid
        sf::RectangleShape line1(sf::Vector2f(600.0f, 10.0f));
        line1.setPosition(0.0f, 0.0f);
        line1.move(300.0f, 100.0f);
        line1.rotate(90.0f);

        sf::RectangleShape line2(sf::Vector2f(600.0f, 10.0f));
        line2.setPosition(0.0f, 0.0f);
        line2.move(600.0f, 100.0f);
        line2.rotate(90.0f);

        sf::RectangleShape line3(sf::Vector2f(850.0f, 10.0f));
        line3.setPosition(0.0f, 0.0f);
        line3.move(20.0f, 300.0f);

        sf::RectangleShape line4(sf::Vector2f(850.0f, 10.0f));
        line4.setPosition(0.0f, 0.0f);
        line4.move(20.0f, 500.0f);

        // draw the lines;
        window.draw(line1);
        window.draw(line2);
        window.draw(line3);
        window.draw(line4);


        //display your move type
        sf::Text your_move;
        your_move.setFont(font_bold);
        you_are = "You are: ";
        you_are.push_back(game.player);
        your_move.setString(you_are);
        your_move.setCharacterSize(24);
        your_move.setPosition(0.0f, 0.0f);
        your_move.move(100.0f, 20.0f);
        window.draw(your_move);
        //text
        sf::Text text;
        text.setFont(font_bold);
        text.setString(game.displayText);
        text.setCharacterSize(24);
        text.setPosition(0.0f, 0.0f);
        text.move(200.0f, 850.0f);
        window.draw(text);

        //draw object on grid based on game.game_grid array
        for (int i = 0; i < 9; ++i) {
            if (game.game_grid[i] != '1') {
                drawXO(i, game.game_grid[i], window);
            }
            
        }
        //end the current frame
        window.display();
    }
    return 0;
}

void drawXO(int grid_pos, char shape_type, sf::RenderWindow& this_window) {
    if (shape_type == 'X') {
        sf::RectangleShape x_1(sf::Vector2f(150.0, 5.0f));
        sf::RectangleShape x_2(sf::Vector2f(150.0, 5.0f));

        x_1.setPosition(0.0f, 0.0f);
        x_2.setPosition(0.0f, 0.0f);

        switch (grid_pos)
        {
        case 0:
            x_1.move(100.0f, 150.0f);
            x_1.rotate(45.0f);
            this_window.draw(x_1);

            x_2.move(100.0f, 255.0f);
            x_2.rotate(-45.0f);
            this_window.draw(x_2);
            break;
        case 1:
            x_1.move(400.0f, 150.0f);
            x_1.rotate(45.0f);
            this_window.draw(x_1);

            x_2.move(400.0f, 255.0f);
            x_2.rotate(-45.0f);
            this_window.draw(x_2);
            break;
        case 2:
            x_1.move(700.0f, 150.0f);
            x_1.rotate(45.0f);
            this_window.draw(x_1);

            x_2.move(700.0f, 255.0f);
            x_2.rotate(-45.0f);
            this_window.draw(x_2);
            break;
        case 3:
            x_1.move(100.0f, 350.0f);
            x_1.rotate(45.0f);
            this_window.draw(x_1);

            x_2.move(100.0f, 455.0f);
            x_2.rotate(-45.0f);
            this_window.draw(x_2);
            break;
        case 4:
            x_1.move(400.0f, 350.0f);
            x_1.rotate(45.0f);
            this_window.draw(x_1);

            x_2.move(400.0f, 455.0f);
            x_2.rotate(-45.0f);
            this_window.draw(x_2);
            break;
        case 5:
            x_1.move(700.0f, 350.0f);
            x_1.rotate(45.0f);
            this_window.draw(x_1);

            x_2.move(700.0f, 455.0f);
            x_2.rotate(-45.0f);
            this_window.draw(x_2);
            break;
        case 6:
            x_1.move(100.0f, 550.0f);
            x_1.rotate(45.0f);
            this_window.draw(x_1);

            x_2.move(100.0f, 655.0f);
            x_2.rotate(-45.0f);
            this_window.draw(x_2);
            break;
        case 7:
            x_1.move(400.0f, 550.0f);
            x_1.rotate(45.0f);
            this_window.draw(x_1);

            x_2.move(400.0f, 655.0f);
            x_2.rotate(-45.0f);
            this_window.draw(x_2);
            break;
        case 8:
            x_1.move(700.0f, 550.0f);
            x_1.rotate(45.0f);
            this_window.draw(x_1);

            x_2.move(700.0f, 655.0f);
            x_2.rotate(-45.0f);
            this_window.draw(x_2);
            break;
        default:
            break;
        }
    }
    else if (shape_type == 'O') {
        sf::CircleShape shape(50.0f);
        shape.setFillColor(sf::Color(0.0f, 0.0f, 0.0f));
        shape.setOutlineThickness(10.0f);
        shape.setOutlineColor(sf::Color(250.0f, 250.0f, 250.0f));
        shape.setPosition(0.0f, 0.0f);
        switch (grid_pos)
        {
        case 0:
            shape.move(100.0f, 150.0f);
            break;
        case 1:
            shape.move(400.0f, 150.0f);
            break;
        case 2:
            shape.move(700.0f, 150.0f);
            break;
        case 3:
            shape.move(100.0f, 350.0f);
            break;
        case 4:
            shape.move(400.0f, 350.0f);
            break;
        case 5:
            shape.move(700.0f, 350.0f);
            break;
        case 6:
            shape.move(100.0f, 550.0f);
            break;
        case 7: 
            shape.move(400.0f, 550.0f);
            break;
        case 8:
            shape.move(700.0f, 550.0f);
            break;
        default:
            break;
        }
        this_window.draw(shape);
    }
}