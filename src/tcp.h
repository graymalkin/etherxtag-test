#ifndef __tcp_h_
#define __tcp_h_

#define ETHER_XTAG_PORT "1337"
#define MAX_TTL			1000			// Time to wait for network in seconds

int connect_to_xtag();
void send_to_xtag(void *data, unsigned int len);
void recv_from_xtag(void *buffer, unsigned int len);

// #define DEBUGGING
#ifdef DEBUGGING
#define DBG(x) x
#else
#define DBG(x)
#endif

#endif // __tcp_h_
