#include <conbee.h>
#include <string.h>
#include <stdio.h>

extern char conbee_device_name[200];

int print_nwk_addr(int argc, char **argv)
{
  struct conbee_device dev;
  uint16_t addr;
  int32_t err;

  err = conbee_connect(&dev, conbee_device_name);
  if (err < 0)
  {
    return -1;
  }

  err = conbee_get_nwk_address(&dev, &addr);
  if (err < 0)
  {
    fprintf(stderr, "Error getting nwk panid\n");
  }
  else
  {
    printf("Current NWK Address: %hd(0x%.4hX)\n", addr, addr);
  }

  conbee_close(&dev);

  return 0;
}
