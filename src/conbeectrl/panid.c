#include <conbee.h>
#include <string.h>
#include <stdio.h>

extern char conbee_device_name[200];

int print_panid(int argc, char **argv)
{
  struct conbee_device dev;
  uint16_t panid;
  int32_t err;

  err = conbee_connect(&dev, conbee_device_name);
  if (err < 0)
  {
    return -1;
  }

  err = conbee_get_nwk_panid(&dev, &panid);
  if (err < 0)
  {
    fprintf(stderr, "Error getting nwk panid\n");
  }
  else
  {
    printf("Current NWK PANID: %hd(0x%.4hX)\n", panid, panid);
  }

  conbee_close(&dev);

  return 0;
}
