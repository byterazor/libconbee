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
 #include <connbee-internal.h>

 /**
 * @brief return the MAC Address of the connbee stick
 *
 */
 int32_t connbee_get_firmware_version(struct connbee_device *dev, struct connbee_version *version)
 {
   struct connbee_frame *request  = connbee_read_firmware_request();
   struct connbee_frame *response;
   uint8_t sequence_number = 0;
   uint8_t err = 0;


   sequence_number = connbee_enqueue_frame(dev, request);

   if(sequence_number < 0)
   {
     return sequence_number;
   }

   err = connbee_wait_for_frame(dev, &response, sequence_number, COMMAND_VERSION);

   err = connbee_get_version(response, version);

   connbee_free_frame(response);

   return err;
 }
