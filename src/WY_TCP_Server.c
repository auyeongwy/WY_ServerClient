/**
 * @file WY_TCP_Server.c
 * This application demonstrates the following:
 * - Starts a TCP server.<br>
 * - Listens for 1 connection.<br>
 * - Echos all messages sent from the connected peer.<br>
 * - If the peer disconnects, return to listening for a TCP connection.<br>
 * - Exits on SIGINT (Ctrl^C).
*/

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <signal.h>
#include <poll.h>


int m_server_socket = -1; /**< The server socket descriptor. */
int m_connected_socket = -1; /**< The connected socket descriptor. */
volatile sig_atomic_t m_exit = 0; /**< Status variable to determine if the application should exit. 1=exit. */


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
    signal(SIGINT, handle_signal); /* Set up signal handler. */

    if(init_tcp_server() == -1) { /* Exit spplication if we cannot even set up basic TCP server functions. */
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
    struct sockaddr_in6 address;
    int tmp = 1;

    if((m_server_socket = socket(AF_INET6, SOCK_STREAM, 0)) == -1) /* Create TCP socket. */
        return -1;

    if(setsockopt(m_server_socket, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(int)) < 0) /* Set port to be immediately reusable after termination. */
        return -1;

    address.sin6_family = AF_INET6;
    address.sin6_port = htons(9234);
    address.sin6_flowinfo = 0;
    //if(inet_pton(AF_INET6, "::1", &(address.sin6_addr)) != 1) /* IPV6 loopback. */
      //  return -1;
    address.sin6_addr = in6addr_any;
    address.sin6_scope_id = 0;

    if(bind(m_server_socket, (struct sockaddr *)&address, sizeof(struct sockaddr_in6)) == -1)
        return -1;

    if(listen(m_server_socket, 0) == -1)
        return -1;

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
    struct sockaddr_in6 address;
    unsigned int tmp = sizeof(struct sockaddr_in6);
    char address_str[INET6_ADDRSTRLEN];

    s_poll_fd.fd = m_server_socket;
    s_poll_fd.events = POLLIN;

    do {
        if(poll(&s_poll_fd, 1, 2000) > 0) /* Waits 2 seconds each call. */
            break;
    } while (m_exit != 1); /* Exit if SIGINT caught or error polling. */
    if(m_exit == 1)
        return;

    if((m_connected_socket = accept(m_server_socket, (struct sockaddr *) &address, &tmp)) == -1) {
        printf("TCP server accept connection failed\n");
        return;
    }

    if(inet_ntop(AF_INET6, &address, address_str, INET6_ADDRSTRLEN) != NULL) {
        printf("Client connected from %s PORT %d\n", address_str, ntohs(address.sin6_port));
    }
    
}


void receive_messages()
{
    char buffer[1024];
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
        m_exit = 1;
    }
}