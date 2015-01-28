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

    RUN_TEST("Testing XScope packet loss", xscope_packet_loss())

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

#define PACKET_SIZE    512
int xscope_test_rate()
{
	char buffer[PACKET_SIZE] = {0};
	struct timespec tsi, tsf;
    clock_gettime(CLOCKTYPE, &tsi);

	int sentCount = 0;
    for(sentCount = 0; sentCount < *xscope_count; sentCount++) 
    {
    	// Print status
        if(!no_spinner)
        {
            printf("\r" TEST_NAME_PREFIX "Testing XScope data-rate :: [%2.2f%% %s]", 
                ((float)sentCount / *xscope_count) * 100, 
                spinner[(sentCount / 2000) % 8]);
            fflush(stdout);
        }
        recv_from_xscope(buffer, PACKET_SIZE);
    }

    clock_gettime(CLOCKTYPE, &tsf);
    double elaps_s = difftime(tsf.tv_sec, tsi.tv_sec);
    long elaps_ns = tsf.tv_nsec - tsi.tv_nsec;
    int bytesSent = sentCount * PACKET_SIZE;
    float mBytesPerS = (bytesSent/((double)(elaps_s + ((double)elaps_ns) / 1.0e9))) / 1000000;

    char string[100] = {0};
    sprintf(string, 
        "Testing XScope data-rate [%2.2f MB/s over %.2fs]    ", 
        mBytesPerS, (double)(elaps_s + ((double)elaps_ns) / 1.0e9));
    printf("\r" TEST_NAME_PREFIX "%-60s :: ", string);

	return PASS;
}

int xscope_packet_loss()
{
    char buffer[PACKET_SIZE] = {0};

    int sentCount = 0;
    int packet_seq = 0;
    int lost_packets = 0;
    for(sentCount = 0; sentCount < *xscope_count; sentCount++) 
    {
        // Print status
        if(!no_spinner)
        {
            printf("\r" TEST_NAME_PREFIX "Testing XScope packet loss :: [%2.2f%% %s]", 
                ((float)sentCount / *xscope_count) * 100, 
                spinner[(sentCount / 2000) % 8]);
            fflush(stdout);
        }
        recv_from_xscope(buffer, PACKET_SIZE);
        if(buffer[0] != (packet_seq + 1))
            lost_packets++;
        packet_seq = buffer[0];
    }

    char string[100] = {0};
    sprintf(string, 
        "Testing XScope packet loss [%d lost (%2.2f%%)]    ", 
        lost_packets, (double)((double)((double)lost_packets / *xscope_count)*100));
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