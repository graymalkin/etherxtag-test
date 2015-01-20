#ifndef __main_h_
#define __main_h_

#define VERSION "0.1"

#define DEBUGGING
#ifdef DEBUGGING
#define DBG(x) x
#else
#define DBG(x)
#endif

int main(int argc, char* argv[]);
void help(char* argv[]);


#endif // __main_h_
