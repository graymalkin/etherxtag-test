#include <string.h>
#include <stdio.h>

#include "dbg_cmd.h"
#include "tcp.h"


int test_all()
{
  printf("Test extract cmd packet :: ");
  if(test_extract_cmd_packet())
    printf("PASS\n");
  else
    printf("FAIL\n");
}


/**
  This test should check that the EtherXTag extracts the dbg_cmd from the
  cmd_packet.
*/
int test_extract_cmd_packet()
{
  dbg_cmd_type_read_mem dbg_cmd = {
    0,                                                // xcore
    0x10000,                                          // addr
    4,                                                // len
    {0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF}  // data
  };

  dbg_cmd_packet dbg_packet = {
    DBG_CMD_READ_MEM_REQ,
    {0, 0x10000, 4, 0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF}
  };

  send_to_xtag(&dbg_packet, sizeof(dbg_packet));

  unsigned int buffer[MAX_DBG_CMD_LEN];

  read_from_xtag(buffer, sizeof(dbg_packet));
  dbg_cmd_packet* response = (dbg_cmd_packet*)&buffer;

  if(response->type == (int) DBG_CMD_READ_MEM_ACK)
    {
      if(!memcmp(response->data, dbg_packet.data, sizeof(dbg_cmd)))
        return 1;
    }

  return 0;
}
