#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "errors.h"
#include "udp.h"

int sfd, rfd;
unsigned rem_sock_size;
fd_set set;
struct timeval timeout;
struct sockaddr_in udp_socket;                 /* our address */
struct sockaddr_in rem_socket;                 /* remote */
struct addrinfo* rp = 0;

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


    struct addrinfo hints;
    memset(&hints,0,sizeof(hints));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;
    hints.ai_flags=AI_ADDRCONFIG;
    int status_code = getaddrinfo(HOST_NAME, ETHER_XSCOPE_PORT, &hints, &rp);
    if (status_code != 0)
    {
        fprintf(stderr, ERROR_MESSAGES[E_FIND_HOST], 
            HOST_NAME, 
            gai_strerror(status_code));
        exit(E_FIND_HOST);
    }

    memcpy(&rem_socket, rp, sizeof(&rp));
    rem_sock_size = rp->ai_addrlen;



    // char addr[16] = {0};
    // inet_ntoa(addr, rem_socket.sin_addr);
    printf("%s\n", inet_ntoa(rem_socket.sin_addr));

    char connect[128] = {0};
    connect[0] = 1;
    send_to_xscope(connect, 128);

    return sfd;
}

void recv_from_xscope(void *buffer, unsigned int len)
{
    if(recvfrom(sfd, buffer, len, 0, rp->ai_addr, &rp->ai_addrlen) == -1)
    // if(recv(sfd, buffer, len, 0) == -1)
    {
    	fprintf(stderr, ERROR_MESSAGES[E_CONNECTION_TIMEOUT], XSCOPE_MAX_TTL);
        exit(E_CONNECTION_TIMEOUT);
    }
}

void send_to_xscope(void *buffer, unsigned int len)
{
    if(sendto(sfd, buffer, len, 0, (struct sockaddr *) rp->ai_addr, rp->ai_addrlen) < 0)
    // if(send(sfd, buffer, len, 0) < 0)
    {
        fprintf(stderr, ERROR_MESSAGES[E_CONNECTION_TIMEOUT], XSCOPE_MAX_TTL);
        exit(E_CONNECTION_TIMEOUT);
    }
}
