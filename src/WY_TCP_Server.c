/**
 * @file WY_TCP_Server.c
 * This application demonstrates the following:
 * - Starts a TCP server.<br>
 * - Listens for 1 connection.<br>
 * - Echos all messages sent from the connected peer.<br>
 * - If the peer disconnects, return to listening for a TCP connection.<br>
 * - Exits on SIGINT (Ctrl^C).<br><br>
 * 
 * Usage:<br>
 * <code>./WY_TCP_Server <IP address> <port></code><br>
 * Example: <br>
 * <code>./WY_TCP_Server 127.0.0.1 11235</code> (Listens on 127.0.0.1 port 11235)<br>
 * OR <br>
 * <code>./WY_TCP_Server</code> (Listens on ::1 port 9234 by default)
*/

#define _POSIX_C_SOURCE 200112L
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <signal.h>
#include <poll.h>
#include <netdb.h>
#include "WY_SC_Helper.h"


int m_server_socket = -1; /**< The server socket descriptor. */
int m_connected_socket = -1; /**< The connected socket descriptor. */
volatile sig_atomic_t m_exit = 0; /**< Status variable to determine if the application should exit. 1=exit. */
struct WY_SC_ADDR m_addr; /**< The address to bind to. */


/**
 * Signal handler.
 * @param p_signal The signal caught as defined by <signal.h>. 
 */
void handle_signal(int p_signal);

/**
 * Initialises the TCP server.
 * @return 0 if OK. Else -1, signalling that something is wrong and the application should exit. 
 */
int init_tcp_server();

/**
 * Closes any TCP socket connections, reverting all variables to default. The server is ready to be restarted again.
 */
void close_tcp_server();

/**
 * Places the server in a state to be ready to accept connections.
 */
void accept_connection();

/**
 * Reapetedly receive messages from a connected socket.
 */
void receive_messages();



int main(int argc, char * argv[])
{
    get_ip_from_args(argc, argv, &m_addr);
    printf("Listening on %s %s\n", m_addr.ip, m_addr.port);

    signal(SIGINT, handle_signal); /* Set up signal handler. */

    if(init_tcp_server(&m_addr) == -1) { /* Exit application if we cannot even set up basic TCP server functions. */
        printf("Init TCP server failed\n");
        return -1;
    }

    while(m_exit != 1) { /* Accept one connection only to receive messages. Repeat until SIGINT is caught. */
        accept_connection();
        receive_messages();
    }

    close_tcp_server();
    return 0;
}


int init_tcp_server()
{
    int tmp = 1;
    struct addrinfo hints, *address=NULL;

    if((m_server_socket = socket(m_addr.ipdomain, SOCK_STREAM, 0)) == -1) /* Create TCP socket. */
        return -1;

    if(setsockopt(m_server_socket, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(int)) < 0) /* Set port to be immediately reusable after termination. */
        return -1;

    memset(&hints, 0x00, sizeof(struct addrinfo));
    hints.ai_family = m_addr.ipdomain;
    hints.ai_socktype = SOCK_STREAM;
    if(getaddrinfo(m_addr.ip, m_addr.port, &hints, &address) != 0) /* Get the address struct for performing connection. */
        return -1;

    if(bind(m_server_socket, address->ai_addr, address->ai_addrlen) == -1) {
        freeaddrinfo(address);
        return -1;
    }

    if(listen(m_server_socket, 0) == -1) {
        freeaddrinfo(address);
        return -1;
    }

    freeaddrinfo(address);
    return 0;
}


void close_tcp_server()
{
    if(m_connected_socket != -1) {
        close(m_connected_socket);
        m_connected_socket = -1;
    }
    if(m_server_socket != -1) {
        close(m_server_socket);
        m_server_socket = -1;
    }
}


void accept_connection()
{
    struct pollfd s_poll_fd;
    unsigned char buf[sizeof(struct sockaddr_in6)];
    struct sockaddr * address = (struct sockaddr *)buf;
    unsigned int tmp = sizeof(struct sockaddr_in6);
    struct WY_SC_ADDR s_peer_addr;


    s_poll_fd.fd = m_server_socket; /* Prepare to accept connection. */
    s_poll_fd.events = POLLIN;
    do {
        if(poll(&s_poll_fd, 1, 2000) > 0) /* Waits 2 seconds each call. */
            break;
    } while (m_exit != 1); /* Exit if SIGINT caught or error polling. */
    if(m_exit == 1)
        return;


    if((m_connected_socket = accept(m_server_socket, address, &tmp)) == -1) { /* The buffers are large enough for either IPv6 or IPv4 address. */ 
        printf("TCP server accept connection failed\n");
        return;
    }

    s_peer_addr.ipdomain = m_addr.ipdomain; /* Set to AF_INET6 or AF_INET. */
    get_address_from_sockaddr(address, &s_peer_addr); /* Get IP address in human-readable form. */
    printf("Client connected from %s : %s\n", s_peer_addr.ip, s_peer_addr.port);
}


void receive_messages()
{
    char buffer[1024]; /* Set a limit to the receive buffer. */
    memset(buffer, 0, 1024);
    ssize_t len = 0;

    while(m_exit!=1) {
        if((len = recv(m_connected_socket, buffer, 1024, MSG_WAITALL)) > 0) {
            printf("%s\n", buffer);
            memset(buffer, 0, len);
        }
        else {
            close(m_connected_socket);
            m_connected_socket = -1;
            break; /* Socket error or disconnected. Exit and let the app handle reconnection attempts. */
        }
    }
}


void handle_signal(int p_signal)
{
    if(p_signal == SIGINT) { /* Catch Ctrl^C. */
        printf("\nCaught SIGINT. Exit.\n");
        m_exit = 1; /* Set the exit status to exit the application. */
    }
}
