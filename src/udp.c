#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

#include "errors.h"
#include "udp.h"

int sfd, rfd;
fd_set set;
struct timeval timeout;
struct sockaddr_in udp_socket;                 /* our address */

int connect_to_xscope(char* HOST_NAME)
{
    timeout.tv_sec = XSCOPE_MAX_TTL;
    timeout.tv_usec = 0;

    // Reset the set of FDs
    FD_ZERO(&set);

    if (!(sfd = socket(AF_INET, SOCK_DGRAM, 0))) {
		fprintf(stderr, ERROR_MESSAGES[E_CREATE_SOCKET]); 	   
	   	exit(E_CREATE_SOCKET);
	}
	
	memset((char *)&udp_socket, 0, sizeof(udp_socket));
    udp_socket.sin_family = AF_INET;
    udp_socket.sin_addr.s_addr = htonl(INADDR_ANY);
    udp_socket.sin_port = htons(ETHER_XSCOPE_PORT_NUM);

    if (bind(sfd, (struct sockaddr *)&udp_socket, sizeof(udp_socket)) < 0) {
    	fprintf(stderr, ERROR_MESSAGES[E_PORT_BIND], HOST_NAME, ETHER_XSCOPE_PORT);
    	exit(E_PORT_BIND);
    }

    if (setsockopt(sfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
	    fprintf(stderr, ERROR_MESSAGES[E_CONNECTION_TIMEOUT]);
        exit(E_CONNECTION_TIMEOUT);
	}


    return sfd;
}

void recv_from_xscope(void *buffer, unsigned int len)
{
    if(recv(sfd, buffer, len, 0) == -1)
    {
    	fprintf(stderr, ERROR_MESSAGES[E_CONNECTION_TIMEOUT], XSCOPE_MAX_TTL);
        exit(E_CONNECTION_TIMEOUT);
    }
}

void send_to_xscope(void *buffer, unsigned int len)
{
    if(sendto(sfd, buffer, len, 0, (struct sockaddr *) &udp_socket, sizeof(udp_socket)) < 0)
    {
        fprintf(stderr, ERROR_MESSAGES[E_CONNECTION_TIMEOUT], XSCOPE_MAX_TTL);
        exit(E_CONNECTION_TIMEOUT);
    }
}
