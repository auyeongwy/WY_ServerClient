/**
 * @file WY_SC_Helper.c
*/
#include "WY_SC_Helper.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


/**
 * Parses IPv6 address information in a struct sockaddr and writes it into a struct WY_SC_ADDR.
 * @param p_sockaddr A valid struct sockaddr containing IPv6 information.
 * @param p_addr Retunrs the IPv6 address in human-readable form. WY_SC_ADDR.ipdomain must first be assigned the value AF_INET6 before calling this function. 
 */
static inline void get_ip6_addr(const struct sockaddr *const p_sockaddr, struct WY_SC_ADDR * restrict p_addr);


/**
 * Parses IPv4 address information in a struct sockaddr and writes it into a struct WY_SC_ADDR.
 * @param p_sockaddr A valid struct sockaddr containing IPv4 information.
 * @param p_addr Retunrs the IPv4 address in human-readable form. WY_SC_ADDR.ipdomain must first be assigned the value AF_INET before calling this function. 
 */
static inline void get_ip4_addr(const struct sockaddr *const p_sockaddr, struct WY_SC_ADDR * restrict p_addr);



void get_ip_from_args(const int argc, char * argv[], struct WY_SC_ADDR * restrict const p_addr)
{
    unsigned char address[sizeof(struct in6_addr)];

    if(argc >= 3) { /* Enough args provided at cmd line, check the values. */
        if(atoi(argv[2]) != 0) { /* Converted the port number successfully. Assume it is valid. */
            strncpy(p_addr->port, argv[2], 7);

            if(inet_pton(AF_INET, argv[1], &address) == 1) {/* Valid IP4 address found. */
                p_addr->ipdomain = AF_INET;
                strncpy(p_addr->ip, argv[1], INET6_ADDRSTRLEN-1);
                return;    
            }
            if(inet_pton(AF_INET6, argv[1], &address) == 1) {/* Valid IP6 address found. */
                p_addr->ipdomain = AF_INET6;
                strncpy(p_addr->ip, argv[1], INET6_ADDRSTRLEN-1);
                return;    
            }
        }
    }

    p_addr->ipdomain = AF_INET6; /* No command line arguments provided or error, so use default values. */
    strncpy(p_addr->port, "9234", 7);
    strncpy(p_addr->ip, "::1", INET6_ADDRSTRLEN-1);
}


void get_address_from_sockaddr(const struct sockaddr *const p_sockaddr, struct WY_SC_ADDR * restrict p_addr)
{
    if(p_addr->ipdomain == AF_INET6)
        get_ip6_addr(p_sockaddr, p_addr);
    else
        get_ip4_addr(p_sockaddr, p_addr);
}


static inline void get_ip6_addr(const struct sockaddr *const p_sockaddr, struct WY_SC_ADDR * restrict p_addr)
{
    const struct sockaddr_in6 * const address = (struct sockaddr_in6 *)p_sockaddr;

    inet_ntop(AF_INET6, &(address->sin6_addr), p_addr->ip, INET6_ADDRSTRLEN-1);
    snprintf(p_addr->port, 8, "%d", ntohs(address->sin6_port));
}


static inline void get_ip4_addr(const struct sockaddr *const p_sockaddr, struct WY_SC_ADDR * restrict p_addr)
{
    const struct sockaddr_in * const address = (struct sockaddr_in *)p_sockaddr;

    inet_ntop(AF_INET, &(address->sin_addr), p_addr->ip, INET_ADDRSTRLEN-1);
    snprintf(p_addr->port, 8, "%d", ntohs(address->sin_port));
}
