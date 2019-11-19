#include <conbee.h>
#include <string.h>
#include <stdio.h>

extern char conbee_device_name[200];

int print_network_mode(int argc, char **argv)
{
  struct conbee_device dev;
  uint8_t mode;
  int32_t err;

  err = conbee_connect(&dev, conbee_device_name);
  if (err < 0)
  {
    return -1;
  }

  err = conbee_get_network_mode(&dev, &mode);
  if (err < 0)
  {
    fprintf(stderr, "Error getting network mode\n");
  }
  else
  {
    printf("Current Network Mode: %s\n", mode == NETWORK_MODE_ROUTER ? "Router" : "Coordinator");
  }

  conbee_close(&dev);

  return 0;
}
