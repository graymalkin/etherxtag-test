#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "main.h"
#include "tcp.h"
#include "udp.h"
#include "errors.h"
#include "xscope_test.h"
#include "xtag_test.h"

#include "test.h"

int quiet_mode;
int no_spinner;

char* HOST_NAME;

int main(int argc, char* argv[]) {
    // Parse the command line options...
    int valid_opts = 0;    
    for(int i = 1; i < argc; i++)
    {        
        if(strstr(argv[i], "-q") || strstr(argv[i],     "--quiet"))
        {
            quiet_mode = 1;
            continue;
        }
        if(strstr(argv[i], "-ns") || strstr(argv[i],    "--no-spinner"))
        {            
            no_spinner = 1;
            continue;
        }
        if(strstr(argv[i], "-h") || strstr(argv[i], "--help"))
        {            
            help(argv);
            exit(E_PRINT_HELP);
        }

        if(!HOST_NAME){
            HOST_NAME = argv[i];
            valid_opts = 1;
            continue;
        }
        valid_opts = 0;
    }
    if(!valid_opts)
    {
        help(argv);
        exit(E_PRINT_HELP);
    }

    int sfd = connect_to_xtag(HOST_NAME);
    xtag_test_all(HOST_NAME);
    close(sfd);

    sfd = connect_to_xscope(HOST_NAME);
    xscope_test_all(HOST_NAME);
    close(sfd);

    printf(ERROR_MESSAGES[NO_ERROR]);
    return NO_ERROR;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat"
void help(char* argv[])
{
    // For some reason GCC incorrectly has issue with the %1$s format...
    printf("Usage: %s <host name> [options]\n"
           "\n"
           "  -h,  --help         Show this help\n"
           "  -ns, --no-spinner   Disable the progress spinner\n"
           "  -q,  --quiet        Quiet mode: disable printing of mismatch data\n"
           "\n"
           "Examples:\n"
           "  Test the device at etherxtag.local quietly:\n"
           "    %1$s etherxtag.local -q\n"
           "  Suitable for testing: only print test results, no spinner:\n"
           "    %1$s etherxtag.local -ns -q\n"
           "\n", argv[0]);
}
#pragma GCC diagnostic pop
