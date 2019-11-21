#include <conbee.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <argparse.h>

extern char conbee_device_name[200];

int set_aps_ext_panid(int argc, char **argv)
{
  struct conbee_device dev;
  uint64_t panid;
  char str_panid[20];
  int32_t err;

  struct arg_parse_ctx *argparse_ctx = argparse_init();

  // argument for router/coordinator
  struct arg_str attr_panid = {
      {ARG_STR,1,0},
      'p',
      "panid",
      str_panid,
      18,
      "the aps ext panid as a hex string, e.g. 0xFFFFFFFFFFFFFFFF (64 bit)",
  };
  argparse_add_string(argparse_ctx, &attr_panid);

  /// parse the commandline
  int ret=argparse_parse(argparse_ctx, argc, argv);

  err = conbee_connect(&dev, conbee_device_name);
  if (err < 0)
  {
    return -1;
  }

  // convert hex string to uint32_t
  panid=strtol(str_panid+2,NULL,16);

  err = conbee_set_aps_extended_panid(&dev, panid);
  if (err < 0)
  {
    fprintf(stderr, "Error setting aps extended panid\n");
  }
  else
  {
    printf("APS EXT PANID set to %s\n", str_panid);
  }

  conbee_close(&dev);

  /// free context
  argparse_free(argparse_ctx);

  return 0;
}
