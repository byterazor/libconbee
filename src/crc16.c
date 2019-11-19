/*
 * This file is part of the libconbee library distribution (https://gitcloud.federationhq.de/byterazor/libconbee)
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

 /** @file */
 
#include <crc16.h>


uint16_t crc16(uint8_t *buffer, uint32_t length)
{
  uint16_t crc  = 0;
  uint8_t  crc0 = 0;
  uint8_t  crc1 = 0;

  while(length--)
  {
    crc += *buffer;
    buffer++;
  }

  crc0 = (~crc + 1) & 0xFF;
  crc1 = ((~crc + 1) >> 8) & 0xFF;

  crc = crc1;
  crc = crc << 8;
  crc +=crc0;

  return crc;
}
