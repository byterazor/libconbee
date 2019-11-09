#ifndef __CONNBEE_H__
#define __CONNBEE_H__
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

/**
 * @defgroup CONNBEE STATUS Codes
 *
 * @{
 */
 /// the requested action was a success
#define STATUS_SUCCESS        0x00

/// the requested action failed
#define STATUS_FAILURE        0x01

/// the requested action is still busy
#define STATUS_BUSY           0x02

/// the request timed out
#define STATUS_TIMEOUT        0x03

/// the request was not supported
#define STATUS_UNSUPPORTED    0x04

/// an error occured while processing the request
#define STATUS_ERROR          0x05

/// no network is available for finishing the request
#define STATUS_NO_NETWORK     0x06

/// a parameter of the request has an invalid value
#define STATUS_INVALID_VALUE  0x07
/** @} */

/**
 * @defgroup CONNBEE Network Status Codes
 *
 * @{
 */

/// the network is offline
#define NETWORK_OFFLINE       0x00

/// the device is joining the network at the moment
#define NETWORK_JOINING       0x01

/// the device is connected to the network
#define NETWORK_CONNECTED     0x02

/// the device is leaving the network at the moment
#define NETWORK_LEAVING       0x03
/** @} */


/**
 * @defgroup CONNBEE Message Command Codes
 *
 * @{
 */

/// request the device state from the stick
#define COMMAND_DEVICE_STATE          0x07

/// request a change of the network state
#define COMMAND_CHANGE_NETWORK_STATE  0x08

/// read a device parameter
#define COMMAND_READ_PARAMETER        0x0A

/// write a device parameter
#define COMMAND_WRITE_PARAMETER       0x0B

/// out of band request from the stick, signalling a device change
#define COMMAND_DEVICE_STATE_CHANGED  0x0E

/// request version information from the stick
#define COMMAND_VERSION               0x0D

/// @todo document as soon as i exactly know what it is doing
#define COMMAND_APS_DATA_REQUEST      0x12

/// @todo document as soon as i exactly know what it is doing
#define COMMAND_APS_DATA_CONFIRM      0x04

/// @todo document as soon as i exactly know what it is doing
#define COMMAND_APS_DATA_INDICATION   0x17
/** @} */


/**
 * @defgroup CONNBEE Platform Identifier
 *
 * @{
 */

/// the used plattform is connbee or RASPBEE
#define PLATFORM_RASPBEE              0x05

/// the used plattform is the connbee2 stick
#define PLATFORM_CONNBEE2             0x07
/** @} */


/**
 * @defgroup CONNBEE TTY connection state
 *
 * @{
 */

/// the TTY is connected
#define TTY_DISCONNECTED              0x00

/// the TTY is not connected
#define TTY_CONNECTED                 0x01
/** @} */

/**
* @brief a connbee device represented by the name of the uart/tty device
*
*/
struct connbee_device
{
  /// the name of the tty device the connbee stick is connected to like /dev/ttyACM0
  char tty[200];

  /// file descriptor to the opened tty device
  int32_t fd;

  /// status of the tty
  int8_t tty_status;

  /// pointer to the connbee_write_byte function
  int32_t (*write_byte)(struct connbee_device *,uint8_t);

  /// pointer to the connbee_read_byte function
  int32_t (*read_byte)(struct connbee_device *,uint8_t *);
};



/**
* @brief a connbee frame buffer
*/
struct connbee_frame
{
  /// the command this frame belongs to
  uint8_t   command;

  /// the sequence number, required for asynchonous data transfer
  uint8_t   sequence_number;

  /// the status of a command response frame, has to be zero on all requests
  uint8_t   status;

  /// the length of the frame buffer
  uint16_t length;

  /// the number of bytes allocated for the payload buffer
  uint16_t payload_length;

  /// the buffer for sending a payload
  uint8_t *payload;

};

/**
* @brief connbee version information
*/
struct connbee_version
{
  /// the major number of the version
  uint8_t major;

  /// the minor number of the version
  uint8_t minor;

  /// which platform is the connbee running on
  uint8_t platform;

  /// has to be initialized to zero
  uint8_t reserved;
};

/**
* @brief function to connect to the connbee stick on the given tty
*
* @param dev     - pointer to a connbee_device structure, please make memory is already allocated
* @param ttyname - c string containing the full path to the tty device of the connbee stick
*
* @return < 0 - an error occured, use errno to get the error code of the system
* @return   0 - everything went fine
*/
int32_t connbee_connect(struct connbee_device *dev, char *ttyname);


/**
* @brief function to close the connection to the connbee stick
*
* @param dev - pointer to the connbee_device structure of the stick to close
*/
void connbee_close(struct connbee_device *dev);

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


/**
* @brief check if the frame status is success
*
* @param frame - pointer to the frame to check
*
* @return 0 - status is not success
* @return 1 - status is success
*/
uint8_t connbee_frame_success(struct connbee_frame *frame);

/**
* @brief check if the frame status is failure
*
* @param frame - pointer to the frame to check
*
* @return 0 - status is not failure
* @return 1 - status is failure
*/
uint8_t connbee_frame_failure(struct connbee_frame *frame);

/**
* @brief check if the frame status is busy
*
* @param frame - pointer to the frame to check
*
* @return 0 - status is not busy
* @return 1 - status is busy
*/
uint8_t connbee_frame_busy(struct connbee_frame *frame);

/**
* @brief check if the frame status is timeout
*
* @param frame - pointer to the frame to check
*
* @return 0 - status is not timeout
* @return 1 - status is timeout
*/
uint8_t connbee_frame_timeout(struct connbee_frame *frame);

/**
* @brief check if the frame status is unsupported
*
* @param frame - pointer to the frame to check
*
* @return 0 - status is not unsupported
* @return 1 - status is unsupported
*/
uint8_t connbee_frame_unsupported(struct connbee_frame *frame);

/**
* @brief check if the frame status is error
*
* @param frame - pointer to the frame to check
*
* @return 0 - status is not error
* @return 1 - status is error
*/
uint8_t connbee_frame_error(struct connbee_frame *frame);

/**
* @brief check if the frame status is nonetwork
*
* @param frame - pointer to the frame to check
*
* @return 0 - status is not nonetwork
* @return 1 - status is nonetwork
*/
uint8_t connbee_frame_nonetwork(struct connbee_frame *frame);

/**
* @brief check if the frame status is invalid value
*
* @param frame - pointer to the frame to check
*
* @return 0 - status is not invalid value
* @return 1 - status is invalid value
*/
uint8_t connbee_frame_invalidvalue(struct connbee_frame *frame);


/**
* @brief get the version information out of a frame containing these information ;)
*
* @param frame    - pointer to the frame with version information
* @param version  - pointer to hold the version information
*
* @return 0   - version information are valid and everything is fine
* @return -1  - no version information could be found
*/
int32_t connbee_get_version(struct connbee_frame *frame, struct connbee_version *version);

#endif
