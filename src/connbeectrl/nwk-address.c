#include <connbee.h>
#include <string.h>
#include <stdio.h>

extern char connbee_device_name[200];

int print_nwk_addr(int argc, char **argv)
{
  struct connbee_device dev;
  uint16_t addr;
  int32_t err;

  err = connbee_connect(&dev, connbee_device_name);
  if (err < 0)
  {
    return -1;
  }

  err = connbee_get_nwk_address(&dev, &addr);
  if (err < 0)
  {
    fprintf(stderr, "Error getting nwk panid\n");
  }
  else
  {
    printf("Current NWK Address: %hd(0x%.4hX)\n", addr, addr);
  }

  connbee_close(&dev);

  return 0;
}
