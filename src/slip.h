#ifndef __SLIP_H__
#define __SLIP_H__
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
 
#include <stdint.h>
#include <conbee.h>

/* SLIP special character codes  */
#define END             0300    /* indicates end of packet */
#define ESC             0333    /* indicates byte stuffing */
#define ESC_END         0334    /* ESC ESC_END means END data byte */
#define ESC_ESC         0335    /* ESC ESC_ESC means ESC data byte */


/**
* @brief transmit one frame of binary data using the slip encoding scheme
*
* the user has to make sure that the buffer really has the number of given bytes,
* otherwise a memory error will occur!
*
* @param dev    - pointer to a conbee_device structure, please make memory is already allocated
* @param buffer - the buffer holding the frame
* @param length - the number of bytes to transmit/ the length of the frame in bytes
*
* @return -1  - something went terribly wrong
* @return >=0 - the number of bytes transmitted
*/
int slip_transmit_packet(struct conbee_device *dev, uint8_t *buffer, uint32_t length);

/**
* @brief transmit one frame of binary data using the slip encoding scheme
*
* the user has to make sure that the buffer really has the number of given bytes,
* otherwise a memory error will occur!
*
* @param dev    - pointer to a conbee_device structure, please make memory is already allocated
* @param buffer - the buffer saving the frame to
* @param length - the number of bytes to receive at maximum/ the length of the frame buffer in bytes
*
* @return -1  - something went terribly wrong
* @return >=0 - the number of bytes received
*/
int slip_receive_packet(struct conbee_device *dev, uint8_t *buffer, uint32_t length);

#endif
