#include <conbee.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <argparse.h>

extern char conbee_device_name[200];

int set_security_mode(int argc, char **argv)
{
  struct conbee_device dev;
  int mode;
  int32_t err;

  struct arg_parse_ctx *argparse_ctx = argparse_init();

  // argument for router/coordinator
  struct arg_int security_mode = {
      {ARG_INT,1,0},
      'm',
      "mode",
      "the channel mask as a hex string, e.g. 0x20000000 (32 bit)",
      &mode
  };
  argparse_add_int(argparse_ctx, &security_mode);

  /// parse the commandline
  int ret=argparse_parse(argparse_ctx, argc, argv);

  err = conbee_connect(&dev, conbee_device_name);
  if (err < 0)
  {
    return -1;
  }

  err = conbee_set_security_mode(&dev, mode);
  if (err < 0)
  {
    fprintf(stderr, "Error setting security mode\n");
  }
  else
  {
    if(mode == SECURITY_MODE_NO)
    {
      printf("Current Security Mode: no security\n");
    }
    else if (mode == SECURITY_MODE_NO_MASTER)
    {
      printf("Current Security Mode: no master, but trust center link key\n");
    }
    else if (mode == SECURITY_MODE_PRECONF)
    {
      printf("Current Security Mode: preconfigured network key\n");
    }
    else if (mode == SECURITY_MODE_TC)
    {
      printf("Current Security Mode: network from trust center\n");
    }
    else
    {
      printf("Current Security Mode: unknon mode\n");
    }
  }

  conbee_close(&dev);

  /// free context
  argparse_free(argparse_ctx);

  return 0;
}
