#include <conbee.h>
#include <string.h>
#include <stdio.h>

extern char conbee_device_name[200];

int print_nwk_ext_panid(int argc, char **argv)
{
  struct conbee_device dev;
  uint64_t panid;
  int32_t err;

  err = conbee_connect(&dev, conbee_device_name);
  if (err < 0)
  {
    return -1;
  }

  err = conbee_get_nwk_extended_panid(&dev, &panid);
  if (err < 0)
  {
    fprintf(stderr, "Error getting nwk panid\n");
  }
  else
  {
    printf("Current NWK EXT PANID: %lu(0x%.8lX)\n", panid, panid);
  }

  conbee_close(&dev);

  return 0;
}
