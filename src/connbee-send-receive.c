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
#include <connbee-send-receive.h>
#include <connbee.h>
#include <connbee-queue.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/select.h>

/**
* @brief manage the asynchronous reception and transmission of connbee frames
*
* @param arg - void pointer to the connbee_device structure
*/
void *connbee_send_receive(void *arg)
{
  struct connbee_device *dev = (struct connbee_device *) arg;
  uint8_t stop=0;
  fd_set rfds;
  struct timeval tv;
  int retval = 0;
  int maxfd = 0;

  // signal that the worker is running
  pthread_mutex_lock(&dev->mutex_worker);
  dev->worker_running=1;
  pthread_mutex_unlock(&dev->mutex_worker);

  // run until stopped
  while(!stop)
  {
    pthread_mutex_lock(&dev->mutex_worker);
    if(dev->worker_stop==1)
    {
      stop=1;
    }
    pthread_mutex_unlock(&dev->mutex_worker);

    // wait for new data on the line from the connbee stick or a transmission request
    FD_ZERO(&rfds);
    FD_SET(dev->fd, &rfds);
    FD_SET(dev->pipe_send_queue[0], &rfds);

    // for select we require the maximum file descriptor
    if(dev->fd > dev->pipe_send_queue[0])
    {
      maxfd=dev->fd;
    }
    else
    {
      maxfd = dev->pipe_send_queue[0];
    }

    // we wait for 1000 usecs
    tv.tv_sec = 0;
    tv.tv_usec = 1000;

    retval = select(maxfd+1, &rfds, NULL, NULL, &tv);

    // if data is available
    if (retval)
    {
      if (FD_ISSET(dev->fd, &rfds))
      {
        // a frame from the connbee stick
        struct connbee_frame *frame = connbee_init_frame();
        retval=connbee_read_frame(dev, frame);

        //todo: check retval
        pthread_mutex_lock(&dev->mutex_receive_queue);
        connbee_queue_push(&dev->receive_queue, (void*) frame);
        pthread_cond_signal(&dev->cond_receive_queue);
        pthread_mutex_unlock(&dev->mutex_receive_queue);

      }
      else
      {
        // a frame to send
        // first clear the pipe
        uint8_t buf[2];
        read(dev->pipe_send_queue[0], buf, 2);

        // get the frame and send
        pthread_mutex_lock(&dev->mutex_send_queue);
        struct connbee_frame *frame = (struct connbee_frame*) connbee_queue_pop(&dev->send_queue);
        pthread_mutex_unlock(&dev->mutex_send_queue);
        if (frame != NULL)
        {
          retval=connbee_write_frame(dev, frame);
          // TODO: check retval
          connbee_free_frame(frame);
        }
      }
    }

  }

  pthread_mutex_lock(&dev->mutex_worker);
  dev->worker_running=0;
  pthread_mutex_unlock(&dev->mutex_worker);

  return NULL;
}
