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
  struct connbee_frame frame;
  struct connbee_version version;

  connbee_connect(&dev,"/dev/ttyACM1");


  frame.command         = COMMAND_VERSION;
  frame.sequence_number = 0;
  frame.status          = 0;
  frame.length          = 5;

  connbee_write_frame(&dev,&frame);
  connbee_read_frame(&dev,&frame);
  connbee_get_version(&frame, &version);

  printf("Major   : %X\n",version.major);
  printf("Minor   : %X\n",version.minor);
  printf("Platform: %X\n",version.platform);


  connbee_close(&dev);


}
