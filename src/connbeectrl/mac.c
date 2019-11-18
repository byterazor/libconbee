#include <connbee.h>
#include <string.h>
#include <stdio.h>

extern char connbee_device_name[200];

int print_mac(int argc, char **argv)
{
  struct connbee_device dev;
  uint8_t mac[8];
  int32_t err;

  err = connbee_connect(&dev, connbee_device_name);
  if (err < 0)
  {
    return -1;
  }

  err = connbee_get_mac_address(&dev,mac);
  if (err < 0)
  {
    fprintf(stderr, "Error getting mac address\n");
  }
  else
  {
    printf("Connbee Stick MAC Address: ");
    for(int i = 0; i < 8; i++)
    {
      printf("%.2hhX%c",mac[i],i<7 ? ':' : ' ');
    }
    printf("\n");
  }

  connbee_close(&dev);

  return 0;
}
