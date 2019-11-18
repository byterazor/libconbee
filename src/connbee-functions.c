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
 #include <string.h>

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

 /**
 * @brief return the MAC Address of the connbee stick
 *
 * @param dev - the device from which to request
 * @param mac - array containing the mac address after calling
 */
 int32_t connbee_get_mac_address(struct connbee_device *dev, uint8_t mac[8])
 {
   struct connbee_frame *request  = connbee_read_parameter_request(PARAM_MAC_ADDRESS);
   struct connbee_frame *response;
   uint8_t sequence_number = 0;
   uint8_t err = 0;

   sequence_number = connbee_enqueue_frame(dev, request);

   if(sequence_number < 0)
   {
     return sequence_number;
   }

   err = connbee_wait_for_frame(dev, &response, sequence_number, COMMAND_READ_PARAMETER);

   if (!connbee_frame_success(response))
   {
     connbee_free_frame(response);
     return -1;
   }

   uint64_t value;
   err = connbee_read_parameter_response_uint64(response, &value);

   memcpy(mac,(void *)&value,8);

   return err;
 }

 /**
 * @brief return the current configured NWK PANID
 *
 * @param dev - the device from to request the PANID
 * @param panid - the returned PANID
 *
 * @return -1 - an error occured
 * @return 0  - everything was fine
 */
 int32_t connbee_get_nwk_panid(struct connbee_device *dev, uint16_t *panid)
 {
   struct connbee_frame *request  = connbee_read_parameter_request(PARAM_NWK_PANID);
   struct connbee_frame *response;
   uint8_t sequence_number = 0;
   uint8_t err = 0;

   sequence_number = connbee_enqueue_frame(dev, request);

   if(sequence_number < 0)
   {
     return sequence_number;
   }

   err = connbee_wait_for_frame(dev, &response, sequence_number, COMMAND_READ_PARAMETER);

   if (!connbee_frame_success(response))
   {
     connbee_free_frame(response);
     return -1;
   }

   err = connbee_read_parameter_response_uint16(response, panid);

   return err;
 }
