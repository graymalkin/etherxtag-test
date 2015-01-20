#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#include "tcp.h"
#include "errors.h"

int sfd;
fd_set set;
struct timeval timeout;

int connect_to_xtag(char* HOST_NAME)
{
    // Reset the set of FDs
    FD_ZERO(&set);

    // Initialize the timeout structure which will be used in the read and write
    //  functions
    timeout.tv_sec = MAX_TTL;
    timeout.tv_usec = 0;

    struct addrinfo hints;
    // Linked list of getaddrinfo results (see: getaddrinfo(3))
    struct addrinfo *result, *rp;
    int status_code = 0;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */

    status_code = getaddrinfo(HOST_NAME, ETHER_XTAG_PORT, &hints, &result);
    if (status_code != 0)
    {
        fprintf(stderr, ERROR_MESSAGES[E_FIND_HOST], 
            HOST_NAME, 
            gai_strerror(status_code));
        exit(E_FIND_HOST);
    }

    DBG(printf("getaddrinfo complete.\n");)

    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1)
            continue;

        if (connect(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
            // We've connected successfully, break out the loop with this FD set
            break; 

        // As we didn't connect successfully close the FD (we'd have hit the
        // break by now if we had)
        close(sfd);
    }
    DBG(printf("Got socket file descriptor.\n");)

    if (rp == NULL)
    {               
        // No address succeeded 
        fprintf(stderr, ERROR_MESSAGES[E_PORT_BIND],
                HOST_NAME, ETHER_XTAG_PORT);
        exit(E_PORT_BIND);
    }
    DBG(printf("Connection complete.\n");)

    freeaddrinfo(result);

    // Add this FD to the set of FDs
    FD_SET(sfd, &set);

    return sfd;
}

void send_to_xtag(void *data, unsigned int len)
{
    // Check that the write FD is available (see: select(2))
    int rv = select(sfd + 1, NULL, &set, NULL, &timeout);

    /* FD is not available */
    if(rv == -1){ 
        fprintf(stderr, ERROR_MESSAGES[E_READ_ERROR]);
        exit(E_READ_ERROR);
    }
    /* FD timed out */
    else if(rv == 0){ 
        fprintf(stderr, ERROR_MESSAGES[E_CONNECTION_TIMEOUT], MAX_TTL);
        exit(E_CONNECTION_TIMEOUT);
    }
    /* FD is available */
    else {
        send(sfd, data, len, 0);
    }
    // usleep(50);
}

void recv_from_xtag(void *buffer, unsigned int len)
{
    // Check that the read FD is available (see: select(2))
    int rv = select(sfd + 1, &set, NULL, NULL, &timeout);

    /* FD is not available */
    if(rv == -1){ 
        fprintf(stderr, ERROR_MESSAGES[E_READ_ERROR]);
        exit(E_READ_ERROR);
    }
    /* FD timed out */
    else if(rv == 0){ 
        fprintf(stderr, ERROR_MESSAGES[E_CONNECTION_TIMEOUT], MAX_TTL);
        exit(E_CONNECTION_TIMEOUT);
    }
    /* FD is available */
    else 
        recv(sfd, buffer, len, 0);
}
