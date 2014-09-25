#ifndef __main_h_
#define __main_h_

#define ETHER_XTAG_PORT "1337"

#define DEBUGGING

#ifdef DEBUGGING
#define DBG(x) x
#else
#define DBG(x)
#endif

int main(int argc, char* argv[]);
int connect_to_xtag();


#endif // __main_h_
