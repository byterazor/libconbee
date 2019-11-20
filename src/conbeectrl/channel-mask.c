#include <conbee.h>
#include <string.h>
#include <stdio.h>

extern char conbee_device_name[200];

int print_channel_mask(int argc, char **argv)
{
  struct conbee_device dev;
  uint32_t mask;
  int32_t err;

  err = conbee_connect(&dev, conbee_device_name);
  if (err < 0)
  {
    return -1;
  }

  err = conbee_get_channel_mask(&dev, &mask);
  if (err < 0)
  {
    fprintf(stderr, "Error getting channel mask\n");
  }
  else
  {
    printf("Current Channel Mask: %u(%.4X)\n", mask,mask);
  }

  conbee_close(&dev);

  return 0;
}
