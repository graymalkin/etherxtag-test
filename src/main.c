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

int sfd;
int quiet_mode, no_spinner;
int xtag_enabled = 1, xscope_enabled = 1;

int batch_count = 10000;
int *xscope_count = &batch_count;
int *xtag_count = &batch_count;

char* HOST_NAME;

int main(int argc, char* argv[]) {
    // Parse the command line options...
    int valid_opts = 0;    
    for(int i = 1; i < argc; i++)
    {        
        if(strstr(argv[i], "-q") || strstr(argv[i], "--quiet"))
        {
            quiet_mode = 1;
            continue;
        }
        if(strstr(argv[i], "-ns") || strstr(argv[i], "--no-spinner"))
        {            
            no_spinner = 1;
            continue;
        }
        if(strstr(argv[i], "-nxt") || strstr(argv[i], "--no-xtag"))
        {
            xtag_enabled = 0;
            continue;
        }
        if(strstr(argv[i], "-nxs") || strstr(argv[i], "--no-xscope"))
        {
            xscope_enabled = 0;
            continue;
        }
        if(strstr(argv[i], "--default-count="))
        {
            char * arg = argv[i];
            char buffer[strlen(argv[i]) - 16];
            strcpy(buffer, arg + 16);

            batch_count = atoi(buffer);
            continue;
        }
        if(strstr(argv[i], "--xtag-count="))
        {
            char * arg = argv[i];
            char buffer[strlen(argv[i]) - 13];
            strcpy(buffer, arg + 13);

            int k = atoi(buffer);
            xtag_count = (int*)malloc(sizeof(int));
            *xtag_count = k;
            continue;
        }
        if(strstr(argv[i], "--xscope-count="))
        {
            char * arg = argv[i];
            char buffer[strlen(argv[i]) - 15];
            strcpy(buffer, arg + 15);

            int k = atoi(buffer);
            xscope_count = (int*)malloc(sizeof(int));
            *xscope_count = k;
            continue;
        }
        if(strstr(argv[i], "-h") || strstr(argv[i], "--help"))
        {
            help(argv);
            exit(E_PRINT_HELP);
        }
        if(strstr(argv[i], "-v") || strstr(argv[i], "--version"))
        {
            printf("EtherXTag Test Version: " VERSION "\n");
            exit(NO_ERROR);
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
        fprintf(stderr, ERROR_MESSAGES[E_INVALID_OPTIONS], argv[0]);
        exit(E_INVALID_OPTIONS);
    }

    if(xtag_enabled)
    {
        sfd = connect_to_xtag(HOST_NAME);
        xtag_test_all(HOST_NAME);
        close(sfd);
    }

    if(xscope_enabled)
    {
        sfd = connect_to_xscope(HOST_NAME);
        xscope_test_all(HOST_NAME);
        close(sfd);
    }

    printf(ERROR_MESSAGES[NO_ERROR]);
    return NO_ERROR;
}

// For some reason GCC incorrectly has issue with the %1$s format...
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat"
void help(char* argv[])
{
    printf("Usage: %s HOST [OPTION...]\n"
           "\n"
           "  -h,   --help         Show this help and exit\n"
           "  -ns,  --no-spinner   Disable the progress spinner\n"
           "  -nxt, --no-xtag      Disable XTAG Testing\n"
           "  -nxs, --no-xscope    Disable XScope Testing\n"
           "  -q,   --quiet        Quiet mode: disable printing of mismatch data\n"
           "  -v,   --version      Print the version and exit\n"
           "  --default-count=<n>  Send <n> packets in each of the batch tests\n"
           "                       (test specific options override this value)\n"
           "  --xscope-count=<n>   Send <n> packets in the XScope batch tests\n"
           "  --xtag-count=<n>     Send <n> packets in the XTAG batch tests\n"
           "\n"
           "\n"
           "Examples:\n"
           "\n"
           "  Test the device at etherxtag.local quietly:\n"
           "    %1$s etherxtag.local -q\n"
           "\n"
           "  Suitable for testing: only print test results, no spinner:\n"
           "    %1$s etherxtag.local -ns -q\n"
           "\n"
           "  Test only the XScope portion of the device:\n"
           "    %1$s etherxtag.local -nxt\n"
           "\n"
           "  Run a full, long test:\n"
           "    %1$s etherxtag.local --default-count=10000\n"
           "\n"
           "\n"
           "More Information:\n"
           "\n"
           "  The program does a variety of tests on the host device. There\n"
           "  are some flags to control them, as specified above. By default\n"
           "  the application will print verbose output and has a run time of\n"
           "  ~10s\n"
           "\n", argv[0]);
}
#pragma GCC diagnostic pop
