#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>
#include "main.h"

char* HOST_NAME = "";
char* buf[1024];

int main(int argc, char* argv[]) {
  if(argc < 2)
    {
      printf("You must provide a host name.\n"
             "  Usage: %s <host name>\n", argv[0]);
       return 1;
    }

  // Fields for storing various things...
  HOST_NAME = argv[1];
  int sfd = connect_to_xtag();
  if(sfd < 0)
    return 1;

  printf("Testing EtherXTAG on Host: %s\n", HOST_NAME);
  DBG(printf("File descriptor = %d\n", sfd);)
  if(write(sfd, "GET\r\n", 6) != 6)
    {
      printf("Error: partial/failed write.\n");
      return 1;
    }

  size_t nread;
  nread = read(sfd, buf, 1024);
  if(nread == -1)
    return 1;

  printf("Received %ld bytes: %s\n", (long)nread, buf);

  return 0;
}

int connect_to_xtag()
{
  struct addrinfo hints;
  struct addrinfo *result, *rp;
  int sfd, s;

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
