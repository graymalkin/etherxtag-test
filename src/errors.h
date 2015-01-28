#ifndef __errors_h_
#define __errors_h_

enum ERROR_CODES {
	NO_ERROR,
	E_PRINT_HELP,
	E_INVALID_OPTIONS,
	E_FIND_HOST,
	E_PORT_BIND,
	E_CREATE_SOCKET,
	E_CONNECTION_DROPPED,
	E_READ_ERROR,
	E_CONNECTION_TIMEOUT,
	E_TEST_FAIL,
	ERROR_COUNT
};

static char * ERROR_MESSAGES[ERROR_COUNT] = {
	// NO_ERROR
	"Program completed successfully.\n",

	// E_PRINT_HELP
	"Help.\n",

	// E_INVALID_OPTIONS
	"Invalid command line options. See `%s --help' for more information.\n"
	"The program will now exit.\n",

	// E_FIND_HOST
	"Error connecting to %s at getaddrinfo: %s.\n"
	"The program will now exit.\n",

	// E_PORT_BIND
	"Error connecting to %s:%s Could not bind port.\n"
	"The program will now exit.\n",

	// E_CREATE_SOCKET
	"Cannot creat socket."
	"The program will now exit.\n",

	// E_CONNECTION_DROPPED
	"Connection to %s dropped.\n"
	"The program will now exit.\n",

	// E_READ_ERROR
	"\nError reading from host.\n"
	"The program will now exit.\n",

	// E_CONNECTION_TIMEOUT
	"\nConnection timed out reading from host (TTL=%ds).\n"
	"The program will now exit.\n",

	// E_TEST_FAIL
	"Tests failed.\n"
	"The program will now exit.\n",
};



#endif // __errors_h_