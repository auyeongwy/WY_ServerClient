/**
 * @file WY_TCP_Client.c
 * This appliation demonstrates the following:
 * - Starts a TCP connection to the WY_TCP_Server application.<br>
 * - Sends a series of messages to the connected server.<br>
 * - Exits.<br>
 * 
 * Usage:<br>
 * <code>./WY_TCP_Client <IP address> <port></code><br>
 * Example: <br>
 * <code>./WY_TCP_Client 127.0.0.1 11235</code> (Connects to 127.0.0.1 port 11235)<br>
 * OR <br>
 * <code>./WY_TCP_Client</code> (Connects to ::1 port 9234 by default)
*/
#define _POSIX_C_SOURCE 200112L
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include "WY_SC_Helper.h"


int m_socket = -1; /**< Connected socket fd. */
struct WY_SC_ADDR m_addr; /**< The address to connect to. */


/**
 * Initialises the connector.
 * @return 0 if success. Else -1.
 */
int init_connector();

/**
 * Connects to the server.
 * @return 0 if success. -1 if error.
 */
int connect_to_server();

/**
 * Sends a series of messages to a connected server via m_socket. This automatically truncates the message to 127 chars if it is longer.
 * @param p_message The message to send.
 */
void send_message(const char *const p_message);

/**
 * A dummy function to receive data on the connected socket. This is a dummy beause in its current implementation it just blocks this application from exiting. Used for debugging and testing purposes at this point.
 */
void receive_message();

/**
 * Closes connection to the server connected by the m_socket fd.
 */
void close_connection();


int main(int argc, char * argv[])
{
    get_ip_from_args(argc, argv, &m_addr);
    printf("Connecting to %s %s\n", m_addr.ip, m_addr.port);

    if(init_connector()== -1) {
        printf("Initialise error\n");
        return 0;
    }
    if(connect_to_server()== -1) {/* Exit if we can't connect to the server. */
        printf("Connect to server error\n");
        return 0;
    }

    send_message("Hello 1\n"); /* Send a bunch of messages on the same connected TCP socket. */
    send_message("Hello 2\n");
    send_message("Hello 3\n");

    //receive_message();
    close_connection(); /* Close the connection. */
    return 0;
}


int init_connector()
{
    // int tmp = 1;
    // char ip[16];
    // struct addrinfo hints, *address=NULL;

    if((m_socket = socket(m_addr.ipdomain, SOCK_STREAM, 0)) == -1) /* Create TCP socket. */
        return -1;

//     if(setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(int)) < 0) /* Set port to be immediately reusable after termination. */
//         return -1;

//     if(m_addr.ipdomain == AF_INET6) { /* Set the IP to initiate connection. */
//         strncpy(ip, "::1", 16);
//     } else {
//         strncpy(ip, "127.0.0.1", 16);
//     }

//     memset(&hints, 0x00, sizeof(struct addrinfo));
//     hints.ai_family = m_addr.ipdomain;
//     hints.ai_socktype = SOCK_STREAM;
//     if(getaddrinfo(ip, "11123", &hints, &address) != 0) /* Get the address to bind to. */
//         return -1;

//    if(bind(m_socket, address->ai_addr, address->ai_addrlen) == -1) { /* Bind to this address for connection. */
//         freeaddrinfo(address);
//         return -1;
//     }    

//     freeaddrinfo(address);
    return 0;  
}


int connect_to_server()
{
    struct addrinfo hints, *address=NULL;

    memset(&hints, 0x00, sizeof(struct addrinfo));
    hints.ai_family = m_addr.ipdomain;
    hints.ai_socktype = SOCK_STREAM;
    //address.ai_flags |= AI_NUMERICHOST;
    if(getaddrinfo(m_addr.ip, m_addr.port, &hints, &address) != 0) /* Get the address struct for performing connection. */
        return -1;

    if(connect(m_socket, address->ai_addr, address->ai_addrlen) != 0) {/* Connect. */
        freeaddrinfo(address);
        return -1;
    }

    freeaddrinfo(address);
    return 0;
}


void close_connection()
{
    if(m_socket != -1) {
        close(m_socket);
        m_socket = -1;
    }
}


void send_message(const char *const p_message)
{
    char buffer[128];
    strncpy(buffer, p_message, 128);
    buffer[127] = 0;

    send(m_socket, buffer, strlen(buffer), 0);
}


void receive_message()
{
    char buffer[128];

    recv(m_socket, buffer, 128, 0);
}
