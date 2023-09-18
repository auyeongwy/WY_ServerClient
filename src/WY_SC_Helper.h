/**
 * @file WY_SC_Helper.h
 * Provides helper functions to reduce code bloat in other parts of the application's code. 
*/

#ifndef _WY_SC_HELPER_H_
#define _WY_SC_HELPER_H_

#include <stdint.h>
#include <sys/socket.h>
#include <arpa/inet.h>


/**
 * Structure for IP information.
 */
struct WY_SC_ADDR
{
    int ipdomain; /**< Set to AF_INET or AF_INET6. */
    char port[8]; /**< Port number as text. */
    char ip[INET6_ADDRSTRLEN]; /**< IP address in IPv4 or IPv6 format. */
};


/**
 * Process the input arguments from the command line and returns the IP and port specified, if any. If those are not specified or there is an  error with the specified arguments, returns IP "::1" and port 9234 by default.
 * 
 * Example:<br>
 * <code>
 * int main(int argc, char *argv[]) { <br>
 *  struct WY_SC_ADDR s_addr; <br>
 *  get_ip_from_args(argc, argv, &s_addr); <br>
 *  printf("Got IP address %s and port %s\n", s_addr.ip, s_addr.port); <br>
 * }
 * </code>
 * @param p_argc The number of arguments passed in at command line.
 * @param p_argv The arguments passed in at command line.
 * @param p_addr Returns the IP address and port specified at command line. If nothing is specified or if any errors are detected such as invalid address format, the default of "::1" nd port "9234" are returned.
 */
void get_ip_from_args(const int p_argc, char * p_argv[], struct WY_SC_ADDR * restrict const p_addr);


/**
 * Given a struct sockaddr filled with either an IPv4 or IPv6 address, extract the IP and port in human-readable form into a struct WY_SC_ADDR.
 * @param p_sockaddr The struct sockaddr to exctract the IP and port information.
 * @param p_addr The extracted information is written into this struct. 
 */
void get_address_from_sockaddr(const struct sockaddr *const p_sockaddr, struct WY_SC_ADDR * restrict p_addr);


#endif