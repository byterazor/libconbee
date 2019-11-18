/*
 * This file is part of the libconnbee2 library distribution (https://gitcloud.federationhq.de/byterazor/libconnbee2)
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
#include <connbeectrl/version.h>
#include <connbeectrl/mac.h>
#include <connbeectrl/panid.h>
#include <connbeectrl/nwk-address.h>
#include <string.h>

char connbee_device_name[200];


int main(int argc, char **argv)
{
  struct arg_parse_ctx *argparse_ctx = argparse_init();

  strncpy(connbee_device_name,"/dev/ttyACM0",12);

  // argument for connbee device name
  struct arg_str device_name = {
      {ARG_STR,1,0},
      'd',
      "device",
      connbee_device_name,
      199,
      "device name of the connbee stick (default: /dev/ttyACM0)"
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

  /// parse the commandline
  int ret=argparse_parse(argparse_ctx, argc, argv);

  /// free context
  argparse_free(argparse_ctx);

}
