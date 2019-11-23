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

 /**
 * @brief return the NWK Address of the stick
 *
 * @param dev - the device from to request the NWK EXTENDED PANID
 * @param panid - pointer to the returned nwk extended panid
 *
 * @return -1 - an error occured
 * @return 0  - everything was fine
 */
 int32_t conbee_get_nwk_extended_panid(struct conbee_device *dev, uint64_t *panid)
 {
   struct conbee_frame *request  = conbee_read_parameter_request(PARAM_NWK_EXT_PANID);
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

   err = conbee_read_parameter_response_uint64(response, panid);

   return err;
 }

 /**
 * @brief return current network mode
 *
 * @param dev - the device from to request the NWK EXTENDED PANID
 * @param mode - pointer to the returned mode
 *
 * @return -1 - an error occured
 * @return 0  - everything was fine
 */
 int32_t conbee_get_network_mode(struct conbee_device *dev, uint8_t *mode)
 {
   struct conbee_frame *request  = conbee_read_parameter_request(PARAM_APS_COORDINATOR);
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

   err = conbee_read_parameter_response_uint8(response, mode);

   return err;
 }

 /**
 * @brief set the network mode
 *
 * @param dev - the device for which to set the network mode
 * @param mode - the mode to set
 *
 * @return -1 - an error occured
 * @return 0  - everything was fine
 */
 int32_t conbee_set_network_mode(struct conbee_device *dev, uint8_t mode)
 {
   struct conbee_frame *request  = conbee_write_parameter_request_uint8(PARAM_APS_COORDINATOR, &mode);
   struct conbee_frame *response;
   uint8_t sequence_number = 0;
   uint8_t err = 0;

   sequence_number = conbee_enqueue_frame(dev, request);

   if(sequence_number < 0)
   {
     return sequence_number;
   }

   err = conbee_wait_for_frame(dev, &response, sequence_number, COMMAND_WRITE_PARAMETER);

   if (!conbee_frame_success(response))
   {
     conbee_free_frame(response);
     return -1;
   }

   conbee_free_frame(response);

   return err;
 }

 /**
 * @brief return current channel mask
 *
 * @param dev - the device from to request the NWK EXTENDED PANID
 * @param mask - pointer to the returned channel mask
 *
 * @return -1 - an error occured
 * @return 0  - everything was fine
 */
 int32_t conbee_get_channel_mask(struct conbee_device *dev, uint32_t *mask)
 {
   struct conbee_frame *request  = conbee_read_parameter_request(PARAM_CHANNEL_MASK);
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

   err = conbee_read_parameter_response_uint32(response, mask);

   return err;
 }

 /**
 * @brief set the channel mask
 *
 * @param dev - the device for which to set the network mode
 * @param mask - the mask to set
 *
 * @return -1 - an error occured
 * @return 0  - everything was fine
 */
 int32_t conbee_set_channel_mask(struct conbee_device *dev, uint32_t mask)
 {
   struct conbee_frame *request  = conbee_write_parameter_request_uint32(PARAM_CHANNEL_MASK, &mask);
   struct conbee_frame *response;
   uint8_t sequence_number = 0;
   uint8_t err = 0;

   sequence_number = conbee_enqueue_frame(dev, request);

   if(sequence_number < 0)
   {
     return sequence_number;
   }

   err = conbee_wait_for_frame(dev, &response, sequence_number, COMMAND_WRITE_PARAMETER);

   if (!conbee_frame_success(response))
   {
     conbee_free_frame(response);
     return -1;
   }

   conbee_free_frame(response);

   return err;
 }

 /**
 * @brief return current aps ext panid
 *
 * @param dev - the device from to request the APS EXTENDED PANID
 * @param mask - pointer to the returned aps ext panid
 *
 * @return -1 - an error occured
 * @return 0  - everything was fine
 */
 int32_t conbee_get_aps_extended_panid(struct conbee_device *dev, uint64_t *panid)
 {
   struct conbee_frame *request  = conbee_read_parameter_request(PARAM_APS_EXT_PANID);
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

   err = conbee_read_parameter_response_uint64(response, panid);

   return err;
 }

 /**
 * @brief set the aps ext panid
 *
 * @param dev - the device for which to set the aps ext panid
 * @param mask - the panid to set
 *
 * @return -1 - an error occured
 * @return 0  - everything was fine
 */
 int32_t conbee_set_aps_extended_panid(struct conbee_device *dev, uint64_t panid)
 {
   struct conbee_frame *request  = conbee_write_parameter_request_uint64(PARAM_APS_EXT_PANID, &panid);
   struct conbee_frame *response;
   uint8_t sequence_number = 0;
   uint8_t err = 0;

   sequence_number = conbee_enqueue_frame(dev, request);

   if(sequence_number < 0)
   {
     return sequence_number;
   }

   err = conbee_wait_for_frame(dev, &response, sequence_number, COMMAND_WRITE_PARAMETER);

   if (!conbee_frame_success(response))
   {
     conbee_free_frame(response);
     return -1;
   }

   conbee_free_frame(response);

   return err;
 }

 /**
 * @brief return current trust center address
 *
 * @param dev - the device from to request the trust center address
 * @param mask - pointer to the returned trust center address
 *
 * @return -1 - an error occured
 * @return 0  - everything was fine
 */
 int32_t conbee_get_trust_center_addr(struct conbee_device *dev, uint64_t *addr)
 {
   struct conbee_frame *request  = conbee_read_parameter_request(PARAM_TRUST_CENTER_ADDRESS);
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

   err = conbee_read_parameter_response_uint64(response, addr);

   return err;
 }

 /**
 * @brief set the trust center address
 *
 * @param dev - the device for which to set the trust center address
 * @param add - the address to set
 *
 * @return -1 - an error occured
 * @return 0  - everything was fine
 */
 int32_t conbee_set_trust_center_addr(struct conbee_device *dev, uint64_t addr)
 {
   struct conbee_frame *request  = conbee_write_parameter_request_uint64(PARAM_TRUST_CENTER_ADDRESS, &addr);
   struct conbee_frame *response;
   uint8_t sequence_number = 0;
   uint8_t err = 0;

   sequence_number = conbee_enqueue_frame(dev, request);

   if(sequence_number < 0)
   {
     return sequence_number;
   }

   err = conbee_wait_for_frame(dev, &response, sequence_number, COMMAND_WRITE_PARAMETER);

   if (!conbee_frame_success(response))
   {
     conbee_free_frame(response);
     return -1;
   }

   conbee_free_frame(response);

   return err;
 }
