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
 #include <conbee.h>
 #include <conbee-internal.h>
 #include <string.h>

 /**
 * @brief return the MAC Address of the conbee stick
 *
 */
 int32_t conbee_get_firmware_version(struct conbee_device *dev, struct conbee_version *version)
 {
   struct conbee_frame *request  = conbee_read_firmware_request();
   struct conbee_frame *response;
   uint8_t sequence_number = 0;
   uint8_t err = 0;


   sequence_number = conbee_enqueue_frame(dev, request);

   if(sequence_number < 0)
   {
     return sequence_number;
   }

   err = conbee_wait_for_frame(dev, &response, sequence_number, COMMAND_VERSION);

   err = conbee_get_version(response, version);

   conbee_free_frame(response);

   return err;
 }

 /**
 * @brief return the MAC Address of the conbee stick
 *
 * @param dev - the device from which to request
 * @param mac - array containing the mac address after calling
 */
 int32_t conbee_get_mac_address(struct conbee_device *dev, uint8_t mac[8])
 {
   struct conbee_frame *request  = conbee_read_parameter_request(PARAM_MAC_ADDRESS);
   struct conbee_frame *response;
   uint8_t sequence_number = 0;
   uint8_t err = 0;

   sequence_number = conbee_enqueue_frame(dev, request);

   if(sequence_number < 0)
   {
     return sequence_number;
   }

   err = conbee_wait_for_frame(dev, &response, sequence_number, COMMAND_READ_PARAMETER);

   if (!conbee_frame_success(response))
   {
     conbee_free_frame(response);
     return -1;
   }

   uint64_t value;
   err = conbee_read_parameter_response_uint64(response, &value);

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
 int32_t conbee_get_nwk_panid(struct conbee_device *dev, uint16_t *panid)
 {
   struct conbee_frame *request  = conbee_read_parameter_request(PARAM_NWK_PANID);
   struct conbee_frame *response;
   uint8_t sequence_number = 0;
   uint8_t err = 0;

   sequence_number = conbee_enqueue_frame(dev, request);

   if(sequence_number < 0)
   {
     return sequence_number;
   }

   err = conbee_wait_for_frame(dev, &response, sequence_number, COMMAND_READ_PARAMETER);

   if (!conbee_frame_success(response))
   {
     conbee_free_frame(response);
     return -1;
   }

   err = conbee_read_parameter_response_uint16(response, panid);

   return err;
 }

 /**
 * @brief return the NWK Address of the stick
 *
 * @param dev - the device from to request the PANID
 * @param addr - pointer to the returned nwk address
 *
 * @return -1 - an error occured
 * @return 0  - everything was fine
 */
 int32_t conbee_get_nwk_address(struct conbee_device *dev, uint16_t *addr)
 {
   struct conbee_frame *request  = conbee_read_parameter_request(PARAM_NWK_ADDRESS);
   struct conbee_frame *response;
   uint8_t sequence_number = 0;
   uint8_t err = 0;

   sequence_number = conbee_enqueue_frame(dev, request);

   if(sequence_number < 0)
   {
     return sequence_number;
   }

   err = conbee_wait_for_frame(dev, &response, sequence_number, COMMAND_READ_PARAMETER);

   if (!conbee_frame_success(response))
   {
     conbee_free_frame(response);
     return -1;
   }

   err = conbee_read_parameter_response_uint16(response, addr);

   return err;
 }
