/*
 * This file is part of the libconbee2 library distribution (https://gitcloud.federationhq.de/byterazor/libconbee2)
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
#include <stdio.h>
#include <argparse.h>
#include <conbeectrl/version.h>
#include <conbeectrl/mac.h>
#include <conbeectrl/panid.h>
#include <conbeectrl/nwk-address.h>
#include <conbeectrl/nwk_ext_panid.h>
#include <conbeectrl/network-mode.h>
#include <conbeectrl/set-network-mode.h>
#include <conbeectrl/channel-mask.h>
#include <conbeectrl/set-channel-mask.h>
#include <string.h>

char conbee_device_name[200];


int main(int argc, char **argv)
{
  struct arg_parse_ctx *argparse_ctx = argparse_init();

  strncpy(conbee_device_name,"/dev/ttyACM0",12);

  // argument for conbee device name
  struct arg_str device_name = {
      {ARG_STR,1,0},
      'd',
      "device",
      conbee_device_name,
      199,
      "device name of the conbee stick (default: /dev/ttyACM0)"
  };
  argparse_add_string(argparse_ctx, &device_name);

  // create a command argument
  struct arg_parse_cmd version_cmd= {
    {0,1,0},                        // 1 = mandatory element
    0,
    "version",                         // command name
    "get firmware version",                  // command description
    &print_version                          // if found call this function
  };

  /// add the argument command to context
  argparse_add_command(argparse_ctx, &version_cmd);

  // create a command argument
  struct arg_parse_cmd mac_cmd= {
    {0,1,0},                        // 1 = mandatory element
    0,
    "mac",                          // command name
    "get sticks mac address",                  // command description
    &print_mac                          // if found call this function
  };

  /// add the argument command to context
  argparse_add_command(argparse_ctx, &mac_cmd);

  // create a command argument
  struct arg_parse_cmd panid_cmd= {
    {0,1,0},                        // 1 = mandatory element
    0,
    "panid",                          // command name
    "get current nwk panid",                  // command description
    &print_panid                          // if found call this function
  };

  /// add the argument command to context
  argparse_add_command(argparse_ctx, &panid_cmd);

  // create a command argument
  struct arg_parse_cmd nwk_addr_cmd= {
    {0,1,0},                        // 1 = mandatory element
    0,
    "nwk-addr",                          // command name
    "get current nwk address",                  // command description
    &print_nwk_addr                          // if found call this function
  };

  /// add the argument command to context
  argparse_add_command(argparse_ctx, &nwk_addr_cmd);

  // create a command argument
  struct arg_parse_cmd nwk_ext_panid_cmd= {
    {0,1,0},                        // 1 = mandatory element
    0,
    "nwk-ext-panid",                          // command name
    "get current nwk ext panid",                  // command description
    &print_nwk_ext_panid                          // if found call this function
  };

  /// add the argument command to context
  argparse_add_command(argparse_ctx, &nwk_ext_panid_cmd);

  // create a command argument
  struct arg_parse_cmd network_mode_cmd= {
    {0,1,0},                        // 1 = mandatory element
    0,
    "network-mode",                          // command name
    "get current network mode",                  // command description
    &print_network_mode                          // if found call this function
  };

  /// add the argument command to context
  argparse_add_command(argparse_ctx, &network_mode_cmd);

  // create a command argument
  struct arg_parse_cmd set_network_mode_cmd= {
    {0,1,0},                        // 1 = mandatory element
    0,
    "set-network-mode",                          // command name
    "set network mode",                  // command description
    &set_network_mode                          // if found call this function
  };

  /// add the argument command to context
  argparse_add_command(argparse_ctx, &set_network_mode_cmd);


  // create a command argument
  struct arg_parse_cmd get_channel_mask_cmd= {
    {0,1,0},                        // 1 = mandatory element
    0,
    "channel-mask",                          // command name
    "print the current channel mask of the stick",                  // command description
    &print_channel_mask                          // if found call this function
  };

  /// add the argument command to context
  argparse_add_command(argparse_ctx, &get_channel_mask_cmd);

  // create a command argument
  struct arg_parse_cmd set_channel_mask_cmd= {
    {0,1,0},                        // 1 = mandatory element
    0,
    "set-channel-mask",                          // command name
    "set channel mask",                  // command description
    &set_channel_mask                          // if found call this function
  };

  /// add the argument command to context
  argparse_add_command(argparse_ctx, &set_channel_mask_cmd);

  /// parse the commandline
  int ret=argparse_parse(argparse_ctx, argc, argv);

  /// free context
  argparse_free(argparse_ctx);

}
