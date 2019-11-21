#include <conbee.h>
#include <string.h>
#include <stdio.h>

extern char conbee_device_name[200];

int print_tc_addr(int argc, char **argv)
{
  struct conbee_device dev;
  uint64_t addr;
  int32_t err;

  err = conbee_connect(&dev, conbee_device_name);
  if (err < 0)
  {
    return -1;
  }

  err = conbee_get_trust_center_addr(&dev, &addr);
  if (err < 0)
  {
    fprintf(stderr, "Error getting trust center address\n");
  }
  else
  {
    printf("Current Trust Center Address: %ld(0x%.16lX)\n", addr, addr);
  }

  conbee_close(&dev);

  return 0;
}
