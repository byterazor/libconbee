#include <conbee.h>
#include <string.h>
#include <stdio.h>

extern char conbee_device_name[200];

int print_security_mode(int argc, char **argv)
{
  struct conbee_device dev;
  uint8_t mode;
  int32_t err;

  err = conbee_connect(&dev, conbee_device_name);
  if (err < 0)
  {
    return -1;
  }

  err = conbee_get_security_mode(&dev, &mode);
  if (err < 0)
  {
    fprintf(stderr, "Error getting security mode\n");
  }
  else
  {
    if(mode == SECURITY_MODE_NO)
    {
      printf("Current Security Mode: no security\n");
    }
    else if (mode == SECURITY_MODE_NO_MASTER)
    {
      printf("Current Security Mode: no master, but trust center link key\n");
    }
    else if (mode == SECURITY_MODE_PRECONF)
    {
      printf("Current Security Mode: preconfigured network key\n");
    }
    else if (mode == SECURITY_MODE_TC)
    {
      printf("Current Security Mode: network from trust center\n");
    }
    else
    {
      printf("Current Security Mode: unknon mode\n");
    }
  }

  conbee_close(&dev);

  return 0;
}
