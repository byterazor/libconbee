#ifndef __CONNBEE_SEND_RECEIVE_H__
#define __CONNBEE_SEND_RECEIVE_H__
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
#include <connbee.h>
#include <connbee-queue.h>

/**
* @brief manage the asynchronous reception and transmission of connbee frames
*
* @param arg - void pointer to the connbee_device structure
*/
void * connbee_send_receive(void *arg);

#endif
