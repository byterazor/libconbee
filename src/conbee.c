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

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <conbee.h>
#include <slip.h>
#include <crc16.h>
#include <arpa/inet.h>
#include <conbee-queue.h>
#include <conbee-internal.h>
#include <conbee-send-receive.h>
#include <pthread.h>
#include <time.h>
/**
* @brief function to connect to the conbee stick on the given tty
*
* @param dev      - pointer to a conbee_device structure, please make memory is already allocated
* @param ttyname  - c string containing the full path to the tty device of the conbee stick
*
* @return < 0 - an error occured, use errno to get the error code of the system
* @return   0 - everything went fine
*/
int32_t conbee_connect(struct conbee_device *dev, char *ttyname)
{
  struct termios tty;
  int32_t err = 0;

  // copy name of tty device into conbee_device structure
  if (strlen(ttyname) < 200)
  {
    memcpy(dev->tty, ttyname, strlen(ttyname));
  }
  else
  {
    memcpy(dev->tty, ttyname, 199);
    dev->tty[199]=0;
  }

  // initialize termios structure
  memset (&tty, 0, sizeof tty);

  // tty is not connected
  dev->tty_status=TTY_DISCONNECTED;

  // open the tty device
  dev->fd = open (ttyname, O_RDWR | O_NOCTTY | O_SYNC);

  if (dev->fd < 0)
  {
    fprintf(stderr,"error: opening %s failed (%s)\n", ttyname, strerror (errno));
    return -1;
  }


  // fetch us the current attributes of the tty
  err = tcgetattr(dev->fd, &tty);

  if (err < 0)
  {
      fprintf(stderr,"error: fetching attributes from %s failed (%s)\n", ttyname, strerror (errno));
      close(dev->fd);
      return -1;
  }

  /*
  * setting input and output baud rate
  */

  err=cfsetospeed (&tty, B115200);

  if (err < 0)
  {
      fprintf(stderr,"error: setting output baudrate of %s failed (%s)\n", ttyname, strerror (errno));
      close(dev->fd);
      return -1;
  }

  err=cfsetispeed (&tty, B115200);

  if (err < 0)
  {
      fprintf(stderr,"error: setting input baudrate of %s failed (%s)\n", ttyname, strerror (errno));
      close(dev->fd);
      return -1;
  }


  // set the configured attributes
  err=tcsetattr (dev->fd, TCSANOW, &tty);

  if (err < 0)
  {
      fprintf(stderr,"error: setting attributes of %s failed (%s)\n", ttyname, strerror (errno));
      close(dev->fd);
      return -1;
  }

  dev->write_byte = &conbee_write_byte;
  dev->read_byte  = &conbee_read_byte;

  // initialize send an receive queues
  conbee_queue_init(&dev->send_queue);
  conbee_queue_init(&dev->receive_queue);

  // initialize mutex to protect queues
  pthread_mutex_init(&dev->mutex_send_queue, NULL);
  pthread_mutex_init(&dev->mutex_receive_queue, NULL);
  pthread_mutex_init(&dev->mutex_send_pipe,NULL);
  pthread_mutex_init(&dev->mutex_sequence_number,NULL);

  dev->sequence_number=0;

  // initialize condition variables to notify listeners to queues
  pthread_cond_init(&dev->cond_receive_queue,NULL);

  err = pipe(dev->pipe_send_queue);
  if (err < 0)
  {
      fprintf(stderr,"error: initializing send queue pipes (%s)\n", strerror (errno));
      close(dev->fd);

      return -1;
  }

  // start the transmission and reception thread
  pthread_mutex_init(&dev->mutex_worker, NULL);
  dev->worker_running=0;
  dev->worker_stop=0;

  pthread_create(&dev->worker, NULL, conbee_send_receive, (void *)dev);

  // set tty as connected
  dev->tty_status=TTY_CONNECTED;

  return 0;
}

/**
* @brief function to close the connection to the conbee stick
*
* @param dev - pointer to the conbee_device structure of the stick to close
*/
void conbee_close(struct conbee_device *dev)
{
  pthread_mutex_lock(&dev->mutex_worker);
  dev->worker_stop=1;
  pthread_mutex_unlock(&dev->mutex_worker);

  uint8_t stopped=0;
  while(!stopped)
  {
    pthread_mutex_lock(&dev->mutex_worker);
    if(dev->worker_running == 0)
    {
      stopped = 1;
    }
    pthread_mutex_unlock(&dev->mutex_worker);
    usleep(100);
  }

  pthread_join(dev->worker,NULL);

  close(dev->fd);

  dev->tty_status = TTY_DISCONNECTED;

}


/**
* @brief helper function to transmit one byte through the tty to the conbee stick
*
* @param dev - pointer to a conbee_device structure, please make sure the device is already connected
* @param  c  - the byte to transfer
*
* @return 0   - everything went fine
* @return -1  - error occured, use errno to find out what
* @return -2  - conbee device is not connected
*/
int32_t conbee_write_byte(struct conbee_device *dev, uint8_t c)
{
  // variable for error codes
  int32_t err = 0;

  if (dev->tty_status == TTY_DISCONNECTED)
  {
    return -2;
  }

  err=write(dev->fd, &c, 1);
  if (err < 0)
  {
      fprintf(stderr,"error: writing %c to %s failed (%s)\n", c, dev->tty, strerror (errno));
      return -1;
  }

  return 0;
}


/**
* @brief helper function to receive one byte from the tty to the conbee stick
*
* @param dev - pointer to a conbee_device structure, please make sure the device is already connected
* @param  c  - the byte to receive
*
* @return 0   - everything went fine
* @return -1  - error occured, use errno to find out what
* @return -2  - conbee device is not connected
*/
int32_t conbee_read_byte(struct conbee_device *dev, uint8_t *c)
{
  // variable for error codes
  int32_t err = 0;

  if (dev->tty_status == TTY_DISCONNECTED)
  {
    return -2;
  }

  err=read(dev->fd, c, 1);
  if (err < 0)
  {
      fprintf(stderr,"error: reading from %s failed (%s)\n", dev->tty, strerror (errno));
      return -1;
  }

  return 0;
}


/**
* @brief write one frame to the conbee stick
*
* @param dev    - the conbee_device to write the frame to, make sure it is already connected
* @param frame  - the frame to write to the conbee device
*
* @return   0 - everything went fine
* @return  -1 - error occured, use ernno to find out what
* @return  -2 - conbee device is not connected
*/
int32_t conbee_write_frame(struct conbee_device *dev, struct conbee_frame *frame)
{
  // variable for error codes
  int32_t err = 0;

  uint8_t buffer[1000];

  if (dev->tty_status == TTY_DISCONNECTED)
  {
    return -2;
  }

  // status is always zero on requests
  frame->status=0;

  // create buffer manually because of alignment of structs
  buffer[0] = frame->command;
  buffer[1] = frame->sequence_number;
  buffer[2] = frame->status;
  memcpy(&buffer[3],&frame->length,sizeof(frame->length));

  if (frame->payload_length > 0)
  {
    memcpy(&buffer[5],&frame->payload_length,sizeof(frame->payload_length));
    memcpy(&buffer[5+sizeof(frame->payload_length)],frame->payload,frame->payload_length);
  }


  // generate crc16
  uint16_t crc = crc16(buffer, frame->length);
  memcpy(&buffer[frame->length],&crc,sizeof(crc));

  slip_transmit_packet(dev,buffer, frame->length+sizeof(crc));


  return 0;
}

/**
* @brief read one frame from the conbee stick
*
* @param dev    - the conbee_device to read the frame from, make sure it is already connected
* @param frame  - the frame read from the conbee device, the function allocates enough space for the payload if required, free it with conbee_free_frame
*
* @return   0 - everything went fine
* @return  -1 - error occured, use ernno to find out what
* @return  -2 - conbee device is not connected
*/
int32_t conbee_read_frame(struct conbee_device *dev, struct conbee_frame *frame)
{
  // variable for error codes
  int32_t err = 0;
  uint8_t buffer[1500];

  if (dev->tty_status == TTY_DISCONNECTED)
  {
    return -2;
  }

  err = slip_receive_packet(dev,buffer,1500);

  if (err > 0)
  {

    // generate the crc and check if is equal
    uint16_t crc  = crc16(buffer,err-sizeof(crc));
    uint16_t crc2;
    memcpy(&crc2,&buffer[err-2],2);

    if (crc != crc2)
    {
      fprintf(stderr,"error: receiving from %s (wrong crc)\n", dev->tty);
      return -2;
    }

    // create the frame from the buffer
    frame->command          = buffer[0];
    frame->sequence_number  = buffer[1];
    frame->status           = buffer[2];

    memcpy(&frame->length,&buffer[3],2);

    // if we have some payload
    if (err > 5)
    {
      frame->payload_length=err-5-sizeof(crc);
      frame->payload = (uint8_t *)malloc(frame->payload_length);
      memcpy(frame->payload, &buffer[5],frame->payload_length);
    }

  }

  return err;
}

/**
* @brief check if the frame status is success
*
* @param frame - pointer to the frame to check
*
* @return 0 - status is not success
* @return 1 - status is success
*/
uint8_t conbee_frame_success(struct conbee_frame *frame)
{
  if (frame->status == STATUS_SUCCESS)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

/**
* @brief check if the frame status is failure
*
* @param frame - pointer to the frame to check
*
* @return 0 - status is not failure
* @return 1 - status is failure
*/
uint8_t conbee_frame_failure(struct conbee_frame *frame)
{
  if (frame->status == STATUS_FAILURE)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

/**
* @brief check if the frame status is busy
*
* @param frame - pointer to the frame to check
*
* @return 0 - status is not busy
* @return 1 - status is busy
*/
uint8_t conbee_frame_busy(struct conbee_frame *frame)
{
  if (frame->status == STATUS_BUSY)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

/**
* @brief check if the frame status is timeout
*
* @param frame - pointer to the frame to check
*
* @return 0 - status is not timeout
* @return 1 - status is timeout
*/
uint8_t conbee_frame_timeout(struct conbee_frame *frame)
{
  if (frame->status == STATUS_TIMEOUT)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

/**
* @brief check if the frame status is unsupported
*
* @param frame - pointer to the frame to check
*
* @return 0 - status is not unsupported
* @return 1 - status is unsupported
*/
uint8_t conbee_frame_unsupported(struct conbee_frame *frame)
{
  if (frame->status == STATUS_UNSUPPORTED)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

/**
* @brief check if the frame status is error
*
* @param frame - pointer to the frame to check
*
* @return 0 - status is not error
* @return 1 - status is error
*/
uint8_t conbee_frame_error(struct conbee_frame *frame)
{
  if (frame->status == STATUS_ERROR)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

/**
* @brief check if the frame status is nonetwork
*
* @param frame - pointer to the frame to check
*
* @return 0 - status is not nonetwork
* @return 1 - status is nonetwork
*/
uint8_t conbee_frame_nonetwork(struct conbee_frame *frame)
{
  if (frame->status == STATUS_NO_NETWORK)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

/**
* @brief check if the frame status is invalid value
*
* @param frame - pointer to the frame to check
*
* @return 0 - status is not invalid value
* @return 1 - status is invalid value
*/
uint8_t conbee_frame_invalidvalue(struct conbee_frame *frame)
{
  if (frame->status == STATUS_INVALID_VALUE)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

/**
* @brief get the version information out of a frame containing these information ;)
*
* @param frame    - pointer to the frame with version information
* @param version  - pointer to hold the version information
*
* @return 0   - version information are valid and everything is fine
* @return -1  - no version information could be found
*/
int32_t conbee_get_version(struct conbee_frame *frame, struct conbee_version *version)
{
  // check if version informaton is available in the frame
  if(frame->command != COMMAND_VERSION && frame->status != STATUS_SUCCESS && frame->length != 9)
  {
    return -1;
  }

  uint32_t help;
  memcpy(&help,frame->payload,sizeof(struct conbee_version));

  help=ntohl(help);
  memcpy(version,&help,sizeof(struct conbee_version));


  return 0;
}

/**
* @brief initialize a basic conbee frame, please always use this method for creating frame
*
* @return pointer to the initialized frame
*/
struct conbee_frame * conbee_init_frame()
{
  struct conbee_frame *frame = malloc(sizeof(struct conbee_frame));

  frame->command          = 0;
  frame->length           = 0;
  frame->sequence_number  = 0;
  frame->status           = 0;
  frame->payload_length   = 0;
  frame->payload          = NULL;

  return frame;
}

/**
* @brief free all memory allocated for the given frame
*
* remember that you can not use the given frame after calling this function.
* otherwise you will get segmentation faults
*
* @param frame - pointer to the frame to free
*/
void conbee_free_frame(struct conbee_frame *frame)
{
  if (frame == NULL)
  {
    return;
  }

  if (frame->payload != NULL)
  {
    frame->payload_length=0;
    free(frame->payload);
  }

  free(frame);
}

/**
* @brief create a frame for requeting the firmware version
*
* make sure to *free* the returned frame after using it! Otherwise you will get memory leaks
*
* @return pointer to the frame for requesting the firmware version
*/
struct conbee_frame * conbee_read_firmware_request()
{
  struct conbee_frame *frame = conbee_init_frame();

  frame->command          = COMMAND_VERSION;
  frame->status           = 0;
  frame->sequence_number  = 0;
  frame->length           = 5;

  return frame;
}

/**
* @brief create a frame for requesting reading a device parameter
*
* make sure to *free* the returned frame after using it! Otherwise you will get memory leaks
*
* @return pointer to the frame for requesting the a device parameter
*/
struct conbee_frame * conbee_read_parameter_request(uint8_t parameter)
{
  struct conbee_frame *frame = conbee_init_frame();

  frame->command          = COMMAND_READ_PARAMETER;
  frame->status           = 0;
  frame->sequence_number  = 0;
  frame->length           = 8;
  frame->payload_length   = 1;
  frame->payload          = malloc(1);
  *frame->payload         = parameter;

  return frame;
}

/**
* @brief parse a read_parameter_response into a uint64_t
*
* @param data - return the value by call by reference
*
* @return   0 - everything worked fine
* @return  -1 - no value of this type available in frame
*/
int32_t conbee_read_parameter_response_uint64(struct conbee_frame *frame, uint64_t *data)
{

  /*
  * a read parameter response always has as payload the payload length and the paramter id
  * therefore, we have to skip the first 3 bytes of the payload
  */

  if (frame->payload_length-(uint16_t)3 != sizeof(uint64_t))
  {
      fprintf(stderr,"error: frame payload has not the exact bytes(%u) for uint64_t(%lu)\n",frame->payload_length-(uint16_t)3,sizeof(uint64_t));
      return -1;
  }

  memcpy(data, &frame->payload[3], sizeof(uint64_t));

  return 0;
}

/**
* @brief parse a read_parameter_response into a uint32_t
*
* @param data - return the value by call by reference
*
* @return   0 - everything worked fine
* @return  -1 - no value of this type available in frame
*/
int32_t conbee_read_parameter_response_uint32(struct conbee_frame *frame, uint32_t *data)
{
  /*
  * a read parameter response always has as payload the payload length and the paramter id
  * therefore, we have to skip the first 3 bytes of the payload
  */

  if (frame->payload_length-3 != sizeof(uint32_t))
  {
      fprintf(stderr,"error: frame payload has not the exact bytes for uint32_t\n");
      return -1;
  }

  memcpy(data, &frame->payload[3], sizeof(uint32_t));

  return 0;
}

/**
* @brief parse a read_parameter_response into a uint16_t
*
* @param data - return the value by call by reference
*
* @return   0 - everything worked fine
* @return  -1 - no value of this type available in frame
*/
int32_t conbee_read_parameter_response_uint16(struct conbee_frame *frame, uint16_t *data)
{
  /*
  * a read parameter response always has as payload the payload length and the paramter id
  * therefore, we have to skip the first 3 bytes of the payload
  */

  if (frame->payload_length-3 != sizeof(uint16_t))
  {
      fprintf(stderr,"error: frame payload has not the exact bytes for uint16_t\n");
      return -1;
  }

  memcpy(data, &frame->payload[3], sizeof(uint16_t));

  return 0;
}

/**
* @brief parse a read_parameter_response into a uint8_t
*
* @param data - return the value by call by reference
*
* @return   0 - everything worked fine
* @return  -1 - no value of this type available in frame
*/
int32_t conbee_read_parameter_response_uint8(struct conbee_frame *frame, uint8_t *data)
{
  /*
  * a read parameter response always has as payload the payload length and the paramter id
  * therefore, we have to skip the first 3 bytes of the payload
  */

  if (frame->payload_length-3 != sizeof(uint8_t))
  {
      fprintf(stderr,"error: frame payload has not the exact bytes(%d) for uint8_t(%lu)\n",frame->payload_length-3,sizeof(uint8_t));
      return -1;
  }

  memcpy(data, &frame->payload[3], sizeof(uint8_t));

  return 0;
}

/**
* @brief cr#include <time.heate a frame for requesting writing a device parameter
*
* make sure to *free* the returned frame after using it! Otherwise you will get memory leaks
*
* @param parameter - the parameter id of the parameter to write to
* @param value     - the value to write to the parameter casted to an uint8_t
* @param value_size- how many bytes is the value large
*
* @return pointer to the frame for requesting the a device parameter
*/
struct conbee_frame * conbee_write_parameter_request(uint8_t parameter, uint8_t *value, uint8_t value_size)
{
  struct conbee_frame *frame = conbee_init_frame();

  frame->command          = COMMAND_WRITE_PARAMETER;
  frame->sequence_number  = 0;
  frame->status           = 0;
  frame->length           = 8 + value_size;
  frame->payload_length   = 1 + value_size;
  frame->payload          = malloc(frame->payload_length);
  frame->payload[0]       = parameter;
  memcpy(&frame->payload[1], value, value_size);


  return frame;
}


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
struct conbee_frame * conbee_write_parameter_request_uint8(uint8_t parameter, uint8_t *value)
{
  return conbee_write_parameter_request(parameter, value, sizeof(uint8_t));
}

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
struct conbee_frame * conbee_write_parameter_request_uint16(uint8_t parameter, uint16_t *value)
{
  return conbee_write_parameter_request(parameter, (uint8_t *)value, sizeof(uint16_t));
}

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
struct conbee_frame * conbee_write_parameter_request_uint32(uint8_t parameter, uint32_t *value)
{
  return conbee_write_parameter_request(parameter, (uint8_t*)value, sizeof(uint32_t));
}

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
struct conbee_frame * conbee_write_parameter_request_uint64(uint8_t parameter, uint64_t *value)
{
  return conbee_write_parameter_request(parameter, (uint8_t*)value, sizeof(uint64_t));
}


/**
* @brief create a frame for requesting the device status
*
* make sure to *free* the returned frame after using it! Otherwise you will get memory leaks
*
* @return pointer to the frame for requesting the device status
*/
struct conbee_frame * conbee_device_status_request()
{
  struct conbee_frame *frame = conbee_init_frame();

  frame->command           = COMMAND_DEVICE_STATE;
  frame->sequence_number   = 0;
  frame->status            = 0;
  frame->length            = 8;
  frame->payload_length    = 3;
  frame->payload           = malloc(3);
  frame->payload[0]        = 0;
  frame->payload[1]        = 0;
  frame->payload[2]        = 0;

  return frame;
}

/**
* @brief create a frame for requesting to create/join a network
*
* make sure to *free* the returned frame after using it! Otherwise you will get memory leaks
*
* @return pointer to the frame for requesting the network creation/joining
*/
struct conbee_frame * conbee_device_network_join_create_request()
{
  struct conbee_frame *frame = conbee_init_frame();

  frame->command           = COMMAND_CHANGE_NETWORK_STATE;
  frame->sequence_number   = 0;
  frame->status            = 0;
  frame->length            = 6;
  frame->payload_length    = 1;
  frame->payload           = malloc(1);
  frame->payload[0]        = NETWORK_CONNECTED;

  return frame;
}

/**
* @brief create a frame for requesting to leave a network
*
* make sure to *free* the returned frame after using it! Otherwise you will get memory leaks
*
* @return pointer to the frame for requesting the network leaving
*/
struct conbee_frame * conbee_device_network_leave_request()
{
  struct conbee_frame *frame = conbee_init_frame();

  frame->command           = COMMAND_CHANGE_NETWORK_STATE;
  frame->sequence_number   = 0;
  frame->status            = 0;
  frame->length            = 6;
  frame->payload_length    = 1;
  frame->payload           = malloc(1);
  frame->payload[0]        = NETWORK_OFFLINE;

  return frame;
}

/**
* @brief parse a device state response and return a device state structure
*
* @param  frame - pointer to the frame to parse
* @param  state - pointer to the state to save the state in
*
* @return   0 - everything worked fine
* @return  -1 - no state information could be found
*/
int32_t conbee_device_state_response(struct conbee_frame *frame, struct conbee_device_state *state)
{

  uint8_t stateByte;

  // there are two responses containing state information
  switch(frame->command)
  {
    case COMMAND_DEVICE_STATE:
                              stateByte = frame->payload[1];
                              break;

    case COMMAND_DEVICE_STATE_CHANGED:
                              stateByte = frame->payload[0];
                              break;

    case COMMAND_APS_DATA_INDICATION:
                              stateByte = frame->payload[3];
                              break;

    default:
                              return -1;
  }

  state->network_state                  = stateByte & 0x03;
  state->apsde_data_confirm             = stateByte & 0x04;
  state->apsde_data_indication          = stateByte & 0x08;
  state->configuration_changed          = stateByte & 0x10;
  state->apsde_data_request_free_slots  = stateByte & 0x20;

  return 0;
}


/**
* @brief create a frame for requesting available APS data
*
* make sure to *free* the returned frame after using it! Otherwise you will get memory leaks
*
* @return pointer to the frame for requesting the aps data
*/
struct conbee_frame * conbee_device_get_aps_data_request()
{
  struct conbee_frame *frame = conbee_init_frame();
  uint8_t flags = 0;

  // at the moment enabling all flags
  flags |= 0x01;
  flags |= 0x02;
  flags |= 0x04;

  frame->command           = COMMAND_APS_DATA_INDICATION;
  frame->sequence_number   = 0;
  frame->status            = 0;
  frame->length            = 8;
  frame->payload_length    = 1;
  frame->payload           = malloc(1);
  frame->payload[0]        = flags;

  return frame;
}

/**
* @brief enqueue a frame for transmission
*
* do not use or free the frame after calling this function
* the frame is freed after the real transmission happened without user intervention
*
* @param dev    - the conbee_device to read the frame from, make sure it is already connected
* @param frame  - the frame to enqueue for transmission
*
* @return   >=0 - the selected sequence number for a request
* @return  -1 - error occured, use ernno to find out what
* @return  -2 - conbee device is not connected
*/
int32_t conbee_enqueue_frame(struct conbee_device *dev, struct conbee_frame *frame)
{
  uint8_t sequence_number = 0;

  pthread_mutex_lock(&dev->mutex_sequence_number);
  sequence_number = dev->sequence_number;
  dev->sequence_number++;
  pthread_mutex_unlock(&dev->mutex_sequence_number);

  // set the sequence number in the frame
  frame->sequence_number=sequence_number;

  pthread_mutex_lock(&dev->mutex_send_queue);
  conbee_queue_push(&dev->send_queue, (void *) frame);
  pthread_mutex_unlock(&dev->mutex_send_queue);

  // wake up transmitter
  uint8_t buf[2]={0x00,0x00};
  pthread_mutex_lock(&dev->mutex_send_pipe);
  write(dev->pipe_send_queue[1],buf,2);
  pthread_mutex_unlock(&dev->mutex_send_pipe);


  return sequence_number;
}


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
int32_t conbee_wait_for_frame(struct conbee_device *dev, struct conbee_frame **frame, uint8_t sequence_number, uint8_t command)
{
  uint8_t found=0;
  struct conbee_frame *help_frame = NULL;
  time_t current_time = time(NULL);

  // lock reception queue and search for message
  pthread_mutex_lock(&dev->mutex_receive_queue);

  while(1)
  {
    struct conbee_queue_item *item = dev->receive_queue.head;

    while(item != NULL)
    {
      help_frame = (struct conbee_frame *) item->contents;

      if (command == COMMAND_ANY && help_frame->sequence_number==sequence_number)
      {
        found = 1;
        conbee_queue_delete(&dev->receive_queue, item);
        break;
      }
      else if (help_frame->command == command && help_frame->sequence_number == sequence_number)
      {
        found = 1;
        conbee_queue_delete(&dev->receive_queue, item);
        break;
      }
      else
      {
        help_frame = NULL;
      }
    }
    pthread_mutex_unlock(&dev->mutex_receive_queue);

    if (found)
    {
      *frame = help_frame;
      return 0;
    }

    pthread_mutex_lock(&dev->mutex_receive_queue);
    pthread_cond_wait(&dev->cond_receive_queue, &dev->mutex_receive_queue);
  }


  return 0;
}
