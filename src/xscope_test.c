#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include "test.h"
#include "udp.h"
#include "xscope_test.h"

int xscope_test_all(char * HOST_NAME)
{
	printf("Testing EtherXScope on Host: %s\n\n", HOST_NAME);

	int count = 0, tot = 0;

	RUN_TEST("Connecting to XScope", xscope_test_connect())
	PRINT_TEST_RESULTS

	return 0;
}

int xscope_test_connect()
{
	char buffer[512] = {0};
	recv_from_xscope(buffer, 512);

	return PASS;
}