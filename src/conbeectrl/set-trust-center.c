#include <conbee.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <argparse.h>

extern char conbee_device_name[200];

int set_trust_center(int argc, char **argv)
{
  struct conbee_device dev;
  uint64_t addr;
  char str_addr[20];
  int32_t err;

  struct arg_parse_ctx *argparse_ctx = argparse_init();

  // argument for router/coordinator
  struct arg_str trust_center_addr = {
      {ARG_STR,1,0},
      'a',
      "addr",
      str_addr,
      18,
      "the trust center address as a hex string, e.g. 0x2000000000000000 (64 bit)",
  };
  argparse_add_string(argparse_ctx, &trust_center_addr);

  /// parse the commandline
  int ret=argparse_parse(argparse_ctx, argc, argv);

  err = conbee_connect(&dev, conbee_device_name);
  if (err < 0)
  {
    return -1;
  }

  // convert hex string to uint32_t
  addr=strtol(str_addr+2,NULL,16);

  err = conbee_set_trust_center_addr(&dev, addr);
  if (err < 0)
  {
    fprintf(stderr, "Error setting trust center\n");
  }
  else
  {
    printf("Trust Center set to %s\n", str_addr);
  }

  conbee_close(&dev);

  /// free context
  argparse_free(argparse_ctx);

  return 0;
}
