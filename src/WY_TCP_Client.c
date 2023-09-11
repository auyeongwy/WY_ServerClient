/**
 * @file WY_TCP_Client.c
 * This appliation demonstrates the following:
 * - Starts a TCP connection to the WY_TCP_Server application.<br>
 * - Sends a series of messages to the connected server.<br>
 * - Exits.
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


int m_socket = -1; /**< Connected socket fd. */


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
 * Closes connection to the server connected by the m_socket fd.
 */
void close_connection();


int main(int argc, char * argv[])
{
    if(connect_to_server()==-1) /* Exit if we can't connect to the server. */
        printf("Connect to server error\n");

    send_message("Hello 1\n"); /* Send a bunch of messages on the same connected TCP socket. */
    send_message("Hello 2\n");
    send_message("Hello 3\n");

    close_connection(); /* Close the connection. */
    return 0;
}


int connect_to_server()
{
    struct addrinfo hints, *address=NULL;

    if((m_socket = socket(AF_INET6, SOCK_STREAM, 0)) == -1) /* Create TCP socket. */
        return -1;

    memset(&hints, 0x00, sizeof(struct addrinfo));
    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_STREAM;
    //address.ai_flags |= AI_NUMERICHOST;
    if(getaddrinfo("localhost", "9234", &hints, &address) != 0) /* Get the address struct for performing connection. */
        return -1;

    if(connect(m_socket, address->ai_addr, address->ai_addrlen) != 0) /* Connect. */
        return -1;

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

