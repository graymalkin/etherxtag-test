#ifndef __tcp_h_
#define __tcp_h_

#define ETHER_XTAG_PORT "1337"

int connect_to_xtag();
int send_to_xtag(void *data, unsigned int len);
void read_from_xtag(void *buffer, unsigned int len);

#ifdef DEBUGGING
#define DBG(x) x
#else
#define DBG(x)
#endif


#endif // __tcp_h_
