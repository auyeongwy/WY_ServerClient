[TOC]
Introduction
============
WY_ServerClient consists of a collection of server-client applications to illustrate the use of socket programming in server client applications.

File Organisation
=================
There are 3 directories in the base directory: 

src: Contains all source code files. <br>

build: Contains the following:
- The Makefile. 
- After compilation, all object, library and demo files will be in this directory. 

doc: Contains the following:
- A README.md and Doxyfile used by doxygen.
- By running doxygen using the provided Doxyfile, all HTML documentation will be generated in this directory.

Compilation and Usage
=====================
Build the library and demo application by entering the build directory and entering "make". This generates:
1. Demo application pairs WY_TCP_Server and WY_TCP_Client.

The supplied Makefile defaults to using the following compiler flags. Modify them as required to suit your own build system.<br>
`-std=c17 -Wall -O2 -march=native`

To clean up object files, run `make clean`. To clean up all files including library files and the demo applications, run `make distclean`.

Demo applications: WY_TCP_Server and WY_TCP_Client
==================================================
WY_TCP_Server:
- WY_TCP_Server starts a TCP server and listens for 1 TCP connection only. 
- By default, WY_TCP_Server will accept command line arguments in the form: 

    `./WY_TCP_Server <IP address> <port>` (Accepts IPv6 or Ipv4 address)<br>
    Example:<br>
    `./WY_TCP_Server 127.0.1 11234` (Listen on 127.0.0.1 port 11234)<br>
    OR<br>
    `./WY_TCP_Server` (Listen on defaults of ::1 port 9234)

- It will echo messages sent by the connected client. There is an internal buffer limit which limits the amount of data received and processed.
- If the client disconnects, it will return to listening for 1 TCP client connection.
- The application exits if it catches a SIGINT (Ctrl^C). 

WY_TCP_Client:
- WY_TCP_Client simply connects to WY_TCP_Server, sends a series of messages and exits.
- By default, WY_TCP_Client will accept command line arguments in the form: 

    `./WY_TCP_Client <IP address> <port>` (Accepts IPv6 or Ipv4 address)<br>
    Example:<br>
    `./WY_TCP_Client 127.0.1 11234` (Connects to 127.0.0.1 port 11234)<br>
    OR<br>
    `./WY_TCP_Client` (Connects to defaults of ::1 port 9234)

The source code for both are easy to read and understand - there is not much more to say about it.
