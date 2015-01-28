#ifndef __udp_h_
#define __udp_h_

#define XSCOPE_MAX_TTL 5
#define ETHER_XSCOPE_PORT "1338" 
#define ETHER_XSCOPE_PORT_NUM 1338


// #define DEBUGGING
#ifdef DEBUGGING
#define DBG(x) x
#else
#define DBG(x)
#endif

int connect_to_xscope(char* HOST_NAME);
void recv_from_xscope(void * buffer, unsigned int len);
void send_to_xscope(void *buffer, unsigned int len);

#endif // __udp_h_