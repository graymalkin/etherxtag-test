#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

#include "tcp.h"

int sfd;

int connect_to_xtag(char* HOST_NAME)
{
  struct addrinfo hints;
  struct addrinfo *result, *rp;
  int s;

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */

  s = getaddrinfo(HOST_NAME, ETHER_XTAG_PORT, &hints, &result);
  if (s != 0)
    {
      printf("Error connecting to %s at getaddrinfo: %s\n"
             "The program will now exit.\n",
             HOST_NAME, gai_strerror(s));
      return -1;
    }
  DBG(printf("getaddrinfo complete.\n");)

  for (rp = result; rp != NULL; rp = rp->ai_next)
    {
      sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
      if (sfd == -1)
        continue;

      if (connect(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
        break;                  /* Success */

      close(sfd);
    }

  if (rp == NULL)
    {               /* No address succeeded */
      printf("Error connecting to %s:%s: Could not bind port\n",
             HOST_NAME, ETHER_XTAG_PORT);
      return -1;
    }
  DBG(printf("connection complete.\n");)
  freeaddrinfo(result);           /* No longer needed */
  return sfd;
}

int send_to_xtag(void *data, unsigned int len)
{
  return write(sfd, data, len);
}

void read_from_xtag(void *buffer, unsigned int len)
{
  read(sfd, buffer, len);
}
