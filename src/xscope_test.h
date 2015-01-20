#ifndef __xscope_test_h_
#define __xscope_test_h_

int xscope_test_all();
int xscope_test_connect();
int xscope_test_rate();
int xscope_packet_loss();
int xscope_close_connection() ;

typedef struct xscope_cmd {
	int cmd;
	char data[124];
} xscope_cmd;

enum xscope_cmd_types {
	XSCOPE_STOP
};

#endif // __xscope_test_h_