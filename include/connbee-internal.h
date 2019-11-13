#ifndef __CONNBEE_INTERNAL_H__
#define __CONNBEE_INTERNAL_H__
/*
 * This file is part of the libconnbee library distribution (https://gitcloud.federationhq.de/byterazor/libconnbee)
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
#include <connbee.h>

/**
* @brief helper function to transmit one byte through the tty to the connbee stick
*
* @param dev - pointer to a connbee_device structure, please make sure the device is already connected
* @param  c  - the byte to transfer
*
* @return 0   - everything went fine
* @return -1  - error occured, use errno to find out what
* @return -2  - connbee device is not connected
*/
int32_t connbee_write_byte(struct connbee_device *dev, uint8_t c);


/**
* @brief helper function to receive one byte from the tty to the connbee stick
*
* @param dev - pointer to a connbee_device structure, please make sure the device is already connected
* @param  c  - the byte to receive
*
* @return 0   - everything went fine
* @return -1  - error occured, use errno to find out what
* @return -2  - connbee device is not connected
*/
int32_t connbee_read_byte(struct connbee_device *dev, uint8_t *c);

/**
* @brief write one frame to the connbee stick
*
* @param dev    - the connbee_device to write the frame to, make sure it is already connected
* @param frame  - the frame to write to the connbee device
*
* @return   0 - everything went fine
* @return  -1 - error occured, use ernno to find out what
* @return  -2 - connbee device is not connected
*/
int32_t connbee_write_frame(struct connbee_device *dev, struct connbee_frame *frame);

/**
* @brief read one frame from the connbee stick
*
* @param dev    - the connbee_device to read the frame from, make sure it is already connected
* @param frame  - the frame read from the connbee device, the function allocates enough space for the payload if required, free it with connbee_free_frame
*
* @return   0 - everything went fine
* @return  -1 - error occured, use ernno to find out what
* @return  -2 - connbee device is not connected
*/
int32_t connbee_read_frame(struct connbee_device *dev, struct connbee_frame *frame);

#endif
