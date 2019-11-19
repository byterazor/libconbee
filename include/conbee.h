#ifndef __CONNBEE_H__
#define __CONNBEE_H__
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
#include <conbee-queue.h>
#include <pthread.h>
#include <unistd.h>

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

/// command representing any command, just used for the wait_for_frame function !!!!
#define COMMAND_ANY                   0xFF
/** @} */


/**
 * @defgroup CONNBEE Platform Identifier
 *
 * @{
 */

/// the used plattform is conbee or RASPBEE
#define PLATFORM_RASPBEE              0x05

/// the used plattform is the conbee2 stick
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
 * @defgroup CONNBEE device parameters
 *
 * @{
 */
/// the MAC address of the device
#define PARAM_MAC_ADDRESS             0x01

/// the zigbee network panid
#define PARAM_NWK_PANID               0x05

/// the zigbee network address
#define PARAM_NWK_ADDRESS             0x07

/// the zigbee network extended panid
#define PARAM_NWK_EXT_PANID           0x08

/// is the device configured as a coordinator
#define PARAM_APS_COORDINATOR         0x09

/// the zigbee channel mask
#define PARAM_CHANNEL_MASK            0x0A

/// the APS extended PANID
#define PARAM_APS_EXT_PANID           0x0B

/// the zigbee address of the trust center
#define PARAM_TRUST_CENTER_ADDRESS    0x0E

/// in which security mode is the network working
#define PARAM_SECURITY_MODE           0x10

/// the network encrypting key
#define PARAM_NETWORK_KEY             0x18

/// the current channel the device is locked on
#define PARAM_CURRENT_CHANNEL         0x1C

/// the implemented protocol version of the device
#define PARAM_PROTOCOL_VERSION        0x22

/// the network update id
#define PARAM_NWK_UPDATE_ID           0x24

/// the watchdog time to live
#define PARAM_WATCHDOG_TTL            0x26

/** @} */

/**
 * @defgroup CONNBEE security modes
 *
 * @{
 */

/// no security enabled
#define SECURITY_MODE_NO              0x00

/// preconfigured network key
#define SECURITY_MODE_PRECONF         0x01

/// network key from trust center
#define SECURITY_MODE_TC              0x02

/// no master but trust center link key
#define SECURITY_MODE_NO_MASTER       0x03

/** @} */


/**
 * @defgroup CONNBEE destination address modes
 *
 * @{
 */

/// use group addressing
#define DEST_ADDR_GROUP               0x01

/// use NWK addressing
#define DEST_ADDR_NWK                 0x02

/// use IEEE addressing, sometimes referred to as MAC addressing
#define DEST_ADDR_IEEE                0x03

/** @} */


/**
 * @defgroup CONNBEE routing
 *
 * @{
 */

/// allow an unlimited number of hops
#define ROUTE_HOP_COUNT_UNLIMITED     0x00

 /** @} */


/**
* @brief a conbee device represented by the name of the uart/tty device
*
*/
struct conbee_device
{
  /// the name of the tty device the conbee stick is connected to like /dev/ttyACM0
  char tty[200];

  /// file descriptor to the opened tty device
  int32_t fd;

  /// status of the tty
  int8_t tty_status;

  /// pointer to the conbee_write_byte function
  int32_t (*write_byte)(struct conbee_device *,uint8_t);

  /// pointer to the conbee_read_byte function
  int32_t (*read_byte)(struct conbee_device *,uint8_t *);

  /// queue for all frames which should be transmitted
  struct conbee_queue_root send_queue;

  /// mutex protecting the send_queue
  pthread_mutex_t mutex_send_queue;

  /// pipe for signalling the transmission process that there is data available
  /// we use a pipe because the select syscall can wait for file descriptors and pipes
  int pipe_send_queue[2];

  /// mutex to protect the send part of the pipe
  pthread_mutex_t mutex_send_pipe;

  /// queue for all frame which have been received
  struct conbee_queue_root receive_queue;

  /// mutex protecting the send_queue
  pthread_mutex_t mutex_receive_queue;

  /// condition variable for signaling changes to the receive queue
  pthread_cond_t cond_receive_queue;

  /// the pthread for managing the transmission and reception of data
  pthread_t worker;

  /// show that worker is running
  uint8_t worker_running;

  /// signal to shutdown the worker
  uint8_t worker_stop;

  /// mutex to protext worker_running and worker_stop
  pthread_mutex_t mutex_worker;

  /// counter for request sequence numbers
  uint8_t sequence_number;

  /// mutex to protect sequence_number
  pthread_mutex_t mutex_sequence_number;

};



/**
* @brief a conbee frame buffer
*/
struct conbee_frame
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
* @brief conbee version information
*/
struct conbee_version
{
  /// the major number of the version
  uint8_t major;

  /// the minor number of the version
  uint8_t minor;

  /// which platform is the conbee running on
  uint8_t platform;

  /// has to be initialized to zero
  uint8_t reserved;
};

/**
* @brief conbee device state
*
*/
struct conbee_device_state
{
  /// current state of the network
  uint8_t network_state;

  /// APSDE-DATA.confirm
  uint8_t apsde_data_confirm;

  /// todo: fill in as soon as i know more about it
  uint8_t apsde_data_indication;

  /// todo: fill in as soon as i know more about it
  uint8_t configuration_changed;

  /// todo: fill in as soon as i know more about it
  uint8_t apsde_data_request_free_slots;
};

/**
* @brief function to connect to the conbee stick on the given tty
*
* @param dev     - pointer to a conbee_device structure, please make memory is already allocated
* @param ttyname - c string containing the full path to the tty device of the conbee stick
*
* @return < 0 - an error occured, use errno to get the error code of the system
* @return   0 - everything went fine
*/
int32_t conbee_connect(struct conbee_device *dev, char *ttyname);


/**
* @brief function to close the connection to the conbee stick
*
* @param dev - pointer to the conbee_device structure of the stick to close
*/
void conbee_close(struct conbee_device *dev);




/**
* @brief enqueue a frame for transmission
*
* do not use or free the frame after calling this function
* the frame is freed after the real transmission happened without user intervention
*
* @param dev    - the conbee_device to read the frame from, make sure it is already connected
* @param frame  - the frame to enqueue for transmission
*
* @return   0 - everything went fine, frame is enqueue
* @return  -1 - error occured, use ernno to find out what
* @return  -2 - conbee device is not connected
*/
int32_t conbee_enqueue_frame(struct conbee_device *dev, struct conbee_frame *frame);

/**
* @brief wait for the reception of a specific frame
*
* free the frame after processing !!!
*
* @param dev                        - the conbee_device to read the frame from, make sure it is already connected
* @param frame                      - the frame received
* @param sequence_number            - wait for this sequence number
* @param command                    - wait for this command type
*
* @return   0 - everything went fine, frame is enqueue
* @return  -1 - error occured, use ernno to find out what
* @return  -2 - conbee device is not connected
*/
int32_t conbee_wait_for_frame(struct conbee_device *dev, struct conbee_frame **frame, uint8_t sequence_number, uint8_t command);


/**
* @brief check if the frame status is success
*
* @param frame - pointer to the frame to check
*
* @return 0 - status is not success
* @return 1 - status is success
*/
uint8_t conbee_frame_success(struct conbee_frame *frame);

/**
* @brief check if the frame status is failure
*
* @param frame - pointer to the frame to check
*
* @return 0 - status is not failure
* @return 1 - status is failure
*/
uint8_t conbee_frame_failure(struct conbee_frame *frame);

/**
* @brief check if the frame status is busy
*
* @param frame - pointer to the frame to check
*
* @return 0 - status is not busy
* @return 1 - status is busy
*/
uint8_t conbee_frame_busy(struct conbee_frame *frame);

/**
* @brief check if the frame status is timeout
*
* @param frame - pointer to the frame to check
*
* @return 0 - status is not timeout
* @return 1 - status is timeout
*/
uint8_t conbee_frame_timeout(struct conbee_frame *frame);

/**
* @brief check if the frame status is unsupported
*
* @param frame - pointer to the frame to check
*
* @return 0 - status is not unsupported
* @return 1 - status is unsupported
*/
uint8_t conbee_frame_unsupported(struct conbee_frame *frame);

/**
* @brief check if the frame status is error
*
* @param frame - pointer to the frame to check
*
* @return 0 - status is not error
* @return 1 - status is error
*/
uint8_t conbee_frame_error(struct conbee_frame *frame);

/**
* @brief check if the frame status is nonetwork
*
* @param frame - pointer to the frame to check
*
* @return 0 - status is not nonetwork
* @return 1 - status is nonetwork
*/
uint8_t conbee_frame_nonetwork(struct conbee_frame *frame);

/**
* @brief check if the frame status is invalid value
*
* @param frame - pointer to the frame to check
*
* @return 0 - status is not invalid value
* @return 1 - status is invalid value
*/
uint8_t conbee_frame_invalidvalue(struct conbee_frame *frame);


/**
* @brief get the version information out of a frame containing these information ;)
*
* @param frame    - pointer to the frame with version information
* @param version  - pointer to hold the version information
*
* @return 0   - version information are valid and everything is fine
* @return -1  - no version information could be found
*/
int32_t conbee_get_version(struct conbee_frame *frame, struct conbee_version *version);


/**
* @brief initialize a basic conbee frame, please always use this method for creating frame
*
* @return pointer to the initialized frame
*/
struct conbee_frame * conbee_init_frame();


/**
* @brief free all memory allocated for the given frame
*
* remember that you can not use the given frame after calling this function.
* otherwise you will get segmentation faults
*
* @param frame - pointer to the frame to free
*/
void conbee_free_frame(struct conbee_frame *frame);

/**
* @brief create a frame for requeting the firmware version
*
* make sure to *free* the returned frame after using it! Otherwise you will get memory leaks
*
* @return pointer to the frame for requesting the firmware version
*/
struct conbee_frame * conbee_read_firmware_request();

/**
* @brief create a frame for requesting reading a device parameter
*
* make sure to *free* the returned frame after using it! Otherwise you will get memory leaks
*
* @return pointer to the frame for requesting the a device parameter
*/
struct conbee_frame * conbee_read_parameter_request(uint8_t parameter);

/**
* @brief create a frame for requesting writing a device parameter
*
* make sure to *free* the returned frame after using it! Otherwise you will get memory leaks
*
* @param parameter - the parameter id of the parameter to write to
* @param value     - the value to write to the parameter casted to an uint8_t
* @param value_size- how many bytes is the value large
*
* @return pointer to the frame for requesting the a device parameter
*/
struct conbee_frame * conbee_write_parameter_request(uint8_t parameter, uint8_t *value, uint8_t value_size);

/**
* @brief create a frame for requesting writing a device parameter of value uint8_t
*
* make sure to *free* the returned frame after using it! Otherwise you will get memory leaks
*
* @param parameter - the parameter id of the parameter to write to
* @param value     - the value to write to the parameter
*
* @return pointer to the frame for requesting the a device parameter
*/
struct conbee_frame * conbee_write_parameter_request_uint8(uint8_t parameter, uint8_t *value);


/**
* @brief create a frame for requesting writing a device parameter of value uint16_t
*
* make sure to *free* the returned frame after using it! Otherwise you will get memory leaks
*
* @param parameter - the parameter id of the parameter to write to
* @param value     - the value to write to the parameter
*
* @return pointer to the frame for requesting the a device parameter
*/
struct conbee_frame * conbee_write_parameter_request_uint16(uint8_t parameter, uint16_t *value);

/**
* @brief create a frame for requesting writing a device parameter of value uint32_t
*
* make sure to *free* the returned frame after using it! Otherwise you will get memory leaks
*
* @param parameter - the parameter id of the parameter to write to
* @param value     - the value to write to the parameter
*
* @return pointer to the frame for requesting the a device parameter
*/
struct conbee_frame * conbee_write_parameter_request_uint32(uint8_t parameter, uint32_t *value);

/**
* @brief create a frame for requesting writing a device parameter of value uint64_t
*
* make sure to *free* the returned frame after using it! Otherwise you will get memory leaks
*
* @param parameter - the parameter id of the parameter to write to
* @param value     - the value to write to the parameter
*
* @return pointer to the frame for requesting the a device parameter
*/
struct conbee_frame * conbee_write_parameter_request_uint64(uint8_t parameter, uint64_t *value);

/**
* @brief create a frame for requesting the device status
*
* make sure to *free* the returned frame after using it! Otherwise you will get memory leaks
*
* @return pointer to the frame for requesting the device status
*/
struct conbee_frame * conbee_device_status_request();

/**
* @brief create a frame for requesting to create/join a network
*
* make sure to *free* the returned frame after using it! Otherwise you will get memory leaks
*
* @return pointer to the frame for requesting the network creation/joining
*/
struct conbee_frame * conbee_device_network_join_create_request();

/**
* @brief create a frame for requesting to leave a network
*
* make sure to *free* the returned frame after using it! Otherwise you will get memory leaks
*
* @return pointer to the frame for requesting the network leaving
*/
struct conbee_frame * conbee_device_network_leave_request();

/**
* @brief create a frame for requesting available APS data
*
* make sure to *free* the returned frame after using it! Otherwise you will get memory leaks
*
* @return pointer to the frame for requesting the aps data
*/
struct conbee_frame * conbee_device_get_aps_data_request();

/**
* @brief parse a read_parameter_response into a uint64_t
*
* @param data - return the value by call by reference
*
* @return   0 - everything worked fine
* @return  -1 - no value of this type available in frame
*/
int32_t conbee_read_parameter_response_uint64(struct conbee_frame *frame, uint64_t *data);

/**
* @brief parse a read_parameter_response into a uint32_t
*
* @param data - return the value by call by reference
*
* @return   0 - everything worked fine
* @return  -1 - no value of this type available in frame
*/
int32_t conbee_read_parameter_response_uint32(struct conbee_frame *frame, uint32_t *data);

/**
* @brief parse a read_parameter_response into a uint16_t
*
* @param data - return the value by call by reference
*
* @return   0 - everything worked fine
* @return  -1 - no value of this type available in frame
*/
int32_t conbee_read_parameter_response_uint16(struct conbee_frame *frame, uint16_t *data);

/**
* @brief parse a read_parameter_response into a uint8_t
*
* @param data - return the value by call by reference
*
* @return   0 - everything worked fine
* @return  -1 - no value of this type available in frame
*/
int32_t conbee_read_parameter_response_uint8(struct conbee_frame *frame, uint8_t *data);


/**
* @brief parse a device state response and return a device state structure
*
* @param  frame - pointer to the frame to parse
* @param  state - pointer to the state to save the state in
*
* @return   0 - everything worked fine
* @return  -1 - no state information could be found
*/
int32_t conbee_device_state_response(struct conbee_frame *frame, struct conbee_device_state *state);



/**
* @brief return the firmware version of the conbee stick
*
*/
int32_t conbee_get_firmware_version(struct conbee_device *dev, struct conbee_version *version);


/**
* @brief return the MAC Address of the conbee stick
*
* @param dev - the device from which to request
* @param mac - array containing the mac address after calling
*/
int32_t conbee_get_mac_address(struct conbee_device *dev, uint8_t mac[8]);


/**
* @brief return the current configured NWK PANID
*
* @param dev - the device from to request the PANID
* @param panid - pointer to the returned PANID
*
* @return -1 - an error occured
* @return 0  - everything was fine
*/
int32_t conbee_get_nwk_panid(struct conbee_device *dev, uint16_t *panid);

/**
* @brief return the NWK Address of the stick
*
* @param dev - the device from to request the PANID
* @param addr - pointer to the returned nwk address
*
* @return -1 - an error occured
* @return 0  - everything was fine
*/
int32_t conbee_get_nwk_address(struct conbee_device *dev, uint16_t *addr);

#endif
