#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include "dbg_cmd.h"
#include "tcp.h"
#include "xtag_test.h"

#include "test.h"

int xtag_test_all(char* HOST_NAME)
{
    printf("Testing EtherXTAG on Host: %s\n\n", HOST_NAME);

    int count = 0;
    int tot = 0;

    /*
     * 
     */
    RUN_TEST("Connecting to device", xtag_test_connect())

    /*
     *  Check the device replies appropriately to a packet. (Using mem_write as example)
     */
    RUN_TEST("Test extract cmd packet", xtag_test_extract_cmd_packet())

    /*
     *  Check the device issues a lock command correctly. 
     */
    RUN_TEST("Test lock device", xtag_test_device_lock_cmd_packet())

    /*
     *  Send large block of data
     */
    RUN_TEST("Send batch data", xtag_test_send_large_batch(HOST_NAME))

    /*
     *  Test getting jtag chain information
     */
    RUN_TEST("Get JTAG chain information", xtag_test_get_jtag_chain_info())

    PRINT_TEST_RESULTS
    return 0;
}


int xtag_test_connect()
{
    dbg_cmd_type_connect dbg_cmd = {
        -1,// int jtag_speed;
        1, // unsigned int debug_enabled;
        0, // unsigned int jtag_devs_pre;
        0, // unsigned int jtag_bits_pre;
        0, // unsigned int jtag_devs_post;
        0, // unsigned int jtag_bits_post;
        0, // unsigned int jtag_max_speed;
        0, // unsigned int enable_digital_scope;
        0, // unsigned int enable_analog_scope;
        {0}// unsigned int data[MAX_DBG_CMD_DATA_LEN-9];
    };
    dbg_cmd_packet dbg_packet = {
        DBG_CMD_CONNECT_REQ
    };
    memcpy(&dbg_packet.data, &dbg_cmd, sizeof(dbg_cmd));
    send_to_xtag(&dbg_packet, sizeof(dbg_packet));

    unsigned int buffer[MAX_DBG_CMD_LEN] = {0};

    recv_from_xtag(buffer, sizeof(dbg_packet));
    dbg_cmd_packet* response = (dbg_cmd_packet*)&buffer;


    unsigned int expected[MAX_DBG_CMD_LEN] = {0};
    expected[0] = 0; expected[1] = 1;

    if(response->type == (int) DBG_CMD_CONNECT_ACK)
        if(memcmp(response->data, expected, MAX_DBG_CMD_LEN) == 0)
            return PASS;

    if(quiet_mode)
        return FAIL;

    printf("\n\n");
    for(int i = 0; i < MAX_DBG_CMD_LEN; i++)
        printf("%d, ", buffer[i]);
    printf("\n");
    for(int i = 0; i < MAX_DBG_CMD_LEN; i++)
        printf("%d, ", expected[i]);

    return FAIL;
}


/**
  This test should check that the EtherXTag extracts the dbg_cmd from the
  cmd_packet.
*/
int xtag_test_extract_cmd_packet()
{
    dbg_cmd_type_write_mem dbg_cmd = {
        0,                                                // xcore
        0x10000,                                          // addr
        4,                                                // len
        {0xBEEFFEED}                                      // data
    };

    dbg_cmd_packet dbg_packet = {
        DBG_CMD_WRITE_MEM_REQ
    };
    memcpy(&dbg_packet.data, &dbg_cmd, sizeof(dbg_cmd));
    send_to_xtag(&dbg_packet, sizeof(dbg_packet));

    unsigned int buffer[MAX_DBG_CMD_LEN] = {0};

    recv_from_xtag(buffer, sizeof(dbg_packet));
    dbg_cmd_packet* response = (dbg_cmd_packet*)&buffer;

    if(response->type == DBG_CMD_WRITE_MEM_ACK)
        return PASS;

    if(quiet_mode)
        return FAIL;

    
    printf("Expected: %d, got: %d", DBG_CMD_WRITE_MEM_ACK, response->type);

    return FAIL;
}

/**
  Check that the lock packet correctly sets the current user. This should also
  test to see if other packets fail. (This is a desirable behaviour, as we don't
  want someone else accessing the device while it is in use.)

  The only other command which should succeed when doing this is the get_devices
  command
*/
int xtag_test_device_lock_cmd_packet() 
{
    dbg_cmd_packet dbg_packet = {
        DBG_CMD_ETHERXTAG_LOCK_REQ,
        {0}
    };
    dbg_cmd_type_etherxtag_lock dbg_cmd = {
        5, {'H', 'a', 'r', 'r', 'y', '\0'}
    };
    memcpy(&dbg_packet.data, &dbg_cmd, 6*sizeof(int));

    send_to_xtag(&dbg_packet, sizeof(dbg_packet));

    unsigned int buffer[MAX_DBG_CMD_LEN] = {0};

    recv_from_xtag(buffer, 32);
    dbg_cmd_packet* response = (dbg_cmd_packet*)&buffer;

    if(response->type == DBG_CMD_ETHERXTAG_LOCK_ACK)
        if(memcmp(response->data, dbg_packet.data, sizeof(dbg_cmd_type_etherxtag_lock)) == 0)
            return PASS;

    if(quiet_mode)
        return FAIL;

    printf("Response type: %d, expected: %d\n", 
        response->type, DBG_CMD_ETHERXTAG_LOCK_ACK);
    for(int i = 0; i < MAX_DBG_CMD_LEN; i++)
        printf("%d, ", buffer[i]);
    return FAIL;
}

int xtag_test_send_large_batch()
{
    dbg_cmd_type_read_mem dbg_cmd = {
        0,                                                // xcore
        0x10000,                                          // addr
        4,                                                // len
        {0xDEADBEEF}                                      // data
    };

    dbg_cmd_packet dbg_packet;
    dbg_packet.type = DBG_CMD_READ_MEM_REQ;
    memcpy(&dbg_packet.data, &dbg_cmd, sizeof(dbg_cmd));

    struct timespec tsi, tsf;

    int success = PASS;
    clock_gettime(CLOCKTYPE, &tsi);
    // Pretty brail spinner.
    char * spinner[8] = { "⣷", "⣯", "⣟", "⡿", "⢿", "⣻", "⣽", "⣾" };


    unsigned int buffer[MAX_DBG_CMD_LEN] = {0};
    int sentCount = 0;
    for(sentCount = 0; 
        sentCount < *xtag_count && success == PASS; 
        sentCount++) 
    {
        // usleep(40000);
        send_to_xtag(&dbg_packet, sizeof(dbg_packet));
        // Print status
        if(!no_spinner)
        {
            printf("\r" TEST_NAME_PREFIX "Send batch data :: [%2.2f%% %s]", 
                ((float)sentCount / *xtag_count) * 100, 
                spinner[(sentCount / 250) % 8]);
            fflush(stdout);
        }

        recv_from_xtag(buffer, sizeof(dbg_packet));
        dbg_cmd_packet* response = (dbg_cmd_packet*)&buffer;

        if(response->type == (int) DBG_CMD_READ_MEM_ACK)
        {
              success = PASS;
        }
    }
    clock_gettime(CLOCKTYPE, &tsf);
    double elaps_s = difftime(tsf.tv_sec, tsi.tv_sec);
    long elaps_ns = tsf.tv_nsec - tsi.tv_nsec;
    int bytesSent = sentCount * 496;
    float bytesPerS = (bytesSent/((double)(elaps_s + ((double)elaps_ns) / 1.0e9))) / 1000;

    char string[100] = {0};
    sprintf(string, 
        "Send batch data [%2.2f kB/s over %2.2fs]    ", 
        bytesPerS, (double)(elaps_s + ((double)elaps_ns) / 1.0e9));
    printf("\r" TEST_NAME_PREFIX "%-60s :: ", string);
    return PASS;
}

int xtag_test_get_jtag_chain_info()
{
    dbg_cmd_packet dbg_packet = {
        DBG_CMD_GET_JTAG_CHAIN_REQ,
        {0}
    };
    dbg_cmd_type_get_jtag_chain dbg_cmd = {
        0
    };
    memcpy(&dbg_packet.data, &dbg_cmd, sizeof(dbg_cmd_type_get_jtag_chain));

    send_to_xtag(&dbg_packet, sizeof(dbg_packet));

    unsigned int buffer[MAX_DBG_CMD_LEN] = {0};    
    recv_from_xtag(buffer, MAX_DBG_CMD_LEN);
    dbg_cmd_packet* response = (dbg_cmd_packet*)&buffer;

    unsigned int expected[MAX_DBG_CMD_LEN] = {0};
    expected[0] = 9779;

    if(response->type == (int) DBG_CMD_GET_JTAG_CHAIN_ACK)
        return PASS;
    
    // We don't actually know what the jtag chain will be like... errrm...
    // hopefully we've passed by now?
    if(memcmp(response->data, expected, MAX_DBG_CMD_LEN) == 0)
        return PASS;

    if(quiet_mode)
        return FAIL;

    printf("Rcv'd: \n");
    for(int i = 0; i < MAX_DBG_CMD_LEN; i++)
        printf("%d, ", buffer[i]);

    printf("\nExpected: \n");
    for(int i = 0; i < MAX_DBG_CMD_LEN; i++)
        printf("%d, ", expected[i]);

    return FAIL;
}
