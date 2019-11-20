#include <conbee.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <argparse.h>

extern char conbee_device_name[200];

int set_channel_mask(int argc, char **argv)
{
  struct conbee_device dev;
  uint32_t mask;
  char str_mask[12];
  int32_t err;

  struct arg_parse_ctx *argparse_ctx = argparse_init();

  // argument for router/coordinator
  struct arg_str channel_mask = {
      {ARG_STR,1,0},
      'm',
      "mask",
      str_mask,
      10,
      "the channel mask as a hex string, e.g. 0x20000000 (32 bit)",
  };
  argparse_add_string(argparse_ctx, &channel_mask);

  /// parse the commandline
  int ret=argparse_parse(argparse_ctx, argc, argv);

  err = conbee_connect(&dev, conbee_device_name);
  if (err < 0)
  {
    return -1;
  }

  // convert hex string to uint32_t
  mask=strtol(str_mask+2,NULL,16);

  err = conbee_set_channel_mask(&dev, mask);
  if (err < 0)
  {
    fprintf(stderr, "Error setting channel mask\n");
  }
  else
  {
    printf("Channel Mask set to %s\n", str_mask);
  }

  conbee_close(&dev);

  /// free context
  argparse_free(argparse_ctx);

  return 0;
}
