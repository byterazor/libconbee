#include <conbee.h>
#include <string.h>
#include <stdio.h>
#include <argparse.h>

extern char conbee_device_name[200];

int set_network_mode(int argc, char **argv)
{
  struct conbee_device dev;
  int mode;
  int32_t err;

  struct arg_parse_ctx *argparse_ctx = argparse_init();

  // argument for router/coordinator
  struct arg_int network_mode = {
      {ARG_INT,1,0},
      'm',
      "mode",
      "the network mode to set 0=Router, 1=Cooardinator",
      &mode,
  };
  argparse_add_int(argparse_ctx, &network_mode);

  /// parse the commandline
  int ret=argparse_parse(argparse_ctx, argc, argv);

  err = conbee_connect(&dev, conbee_device_name);
  if (err < 0)
  {
    return -1;
  }

  err = conbee_set_network_mode(&dev, mode);
  if (err < 0)
  {
    fprintf(stderr, "Error setting network mode\n");
  }
  else
  {
    printf("Network mode set to %s\n", mode == 0 ? "Router" : "Coordinator");
  }

  conbee_close(&dev);

  /// free context
  argparse_free(argparse_ctx);

  return 0;
}
