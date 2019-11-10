/*
 * This file is part of the libconnbee2 library distribution (https://gitcloud.federationhq.de/byterazor/libconnbee2)
 * Copyright (c) 2019 Dominik Meyer <dmeyer@federationhq.de>.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdio.h>
#include <connbee.h>


int main()
{

  struct connbee_device dev;
  struct connbee_frame *frame;
  struct connbee_frame *response;
  int32_t err=0;

  connbee_connect(&dev,"/dev/ttyACM0");
  printf("Connected\n");

  frame     = connbee_device_status_request();
  response  = connbee_init_frame();

  connbee_write_frame(&dev,frame);
  connbee_read_frame(&dev, response);

  if(connbee_frame_success(response))
  {
    printf("payload: ");
    for(int i=0; i<response->payload_length; i++)
    {
      printf("%.2X ", response->payload[i]);
    }
    printf("\n");
  }

  connbee_free_frame(frame);
  connbee_free_frame(response);


  connbee_close(&dev);


}
