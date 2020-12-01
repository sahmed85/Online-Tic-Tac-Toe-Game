*Please refer to the documentation for more in-depth description and reference*
# Introduction
This documentation is a reference on how to setup and run my final project for ECE 4122 (Fall 2020) on Microsoft Visual Studio Code for Windows. This documentation covers how to install need libraries and run the code to verify its functionality. 
This project is an online Tic-Tac-Toe game with server and client executables. This project creates a server that can handle multiple matchmaking sessions and spawn players into matches. This project also incorporates a client side that connects to the server, waits for matchmaking to complete, and spawn the graphics for playing the game. Gameplay graphics utilizes SFML library (which uses OpenGL) to handle window, keyboard, mouse clicks and graphics on the tic-tac-toe board. Communication between players is handled by the server (as shown in the Figure 1) and utilizes UDP to communicate across client-server-client. 

# Future Fixes 
I wanted to take some time to explain fixes that I would do if I had more time:
  •	Utilize TCP for matchmaking request and then UDP for all other message types.
  •	More graceful exit for player window:
    o	Currently an error is thrown once the client game is exited. I wish I could spend more time debugging this issue (however, server is still listening for matchmaking).
  •	Enable a restart feature, to re-enter matchmaking or replay the person that you just played.
  •	Optimize a few of the helper functions. 

