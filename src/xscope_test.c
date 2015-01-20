#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include "test.h"
#include "udp.h"
#include "xscope_test.h"
char * spinner[8] = { "⣷", "⣯", "⣟", "⡿", "⢿", "⣻", "⣽", "⣾" };

int xscope_test_all(char * HOST_NAME)
{
	printf("Testing EtherXScope on Host: %s\n\n", HOST_NAME);

	int count = 0, tot = 0;

	RUN_TEST("Connecting to XScope", xscope_test_connect())

	RUN_TEST("Testing XScope data-rate", xscope_test_rate())

    RUN_TEST("Testing XScope close connection", xscope_close_connection())

	PRINT_TEST_RESULTS

	return 0;
}

int xscope_test_connect()
{
	char buffer[512] = {0};
	recv_from_xscope(buffer, 512);

	return PASS;
}

#define MAX_TEST_COUNT 10000
#define PACKET_SIZE    512
int xscope_test_rate()
{
	char buffer[PACKET_SIZE] = {0};
	struct timespec tsi, tsf;
    clock_gettime(CLOCKTYPE, &tsi);

	int sentCount = 0;
    for(sentCount = 0; sentCount < MAX_TEST_COUNT; sentCount++) 
    {
    	// Print status
        if(!no_spinner)
        {
            printf("\r" TEST_NAME_PREFIX "Testing XScope data-rate :: [%.2f%% %s]", 
                ((float)sentCount / MAX_TEST_COUNT) * 100, 
                spinner[(sentCount / 2000) % 8]);
            fflush(stdout);
        }
        recv_from_xscope(buffer, PACKET_SIZE);
    }

    clock_gettime(CLOCKTYPE, &tsf);
    double elaps_s = difftime(tsf.tv_sec, tsi.tv_sec);
    long elaps_ns = tsf.tv_nsec - tsi.tv_nsec;
    int bytesSent = sentCount * PACKET_SIZE;
    float bytesPerS = (bytesSent/((double)(elaps_s + ((double)elaps_ns) / 1.0e9))) / 1000;

    char string[100] = {0};
    sprintf(string, 
        "Send batch data [%.2f kB/s over %.2fs]    ", 
        bytesPerS, (double)(elaps_s + ((double)elaps_ns) / 1.0e9));
    printf("\r" TEST_NAME_PREFIX "%-60s :: ", string);


	return PASS;
}

int xscope_close_connection() 
{
    // To tell the debugger that we're done, we should send a "stop" packet
    xscope_cmd xsc;
    xsc.cmd = XSCOPE_STOP;

    send_to_xscope(&xsc, sizeof(xsc));

    return PASS;
}