#include <conbee.h>
#include <string.h>
#include <stdio.h>

extern char conbee_device_name[200];

int print_version(int argc, char **argv)
{
  struct conbee_device dev;
  struct conbee_version version;
  int32_t err;

  err = conbee_connect(&dev, conbee_device_name);
  if (err < 0)
  {
    return -1;
  }

  err = conbee_get_firmware_version(&dev, &version);
  if (err < 0)
  {
    fprintf(stderr, "Error getting firmware version\n");
  }
  else
  {
    printf("Connbee Firmware Version\n");
    printf("\tMajor Nr  : %.2X\n",version.major);
    printf("\tMinor Nr  : %.2X\n",version.minor);
    printf("\tPlattform : %s\n", version.platform == PLATFORM_RASPBEE ? "RaspBee" : "Connbee2");
  }

  conbee_close(&dev);

  return 0;
}
