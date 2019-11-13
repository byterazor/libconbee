/*
YAAP - Yet Another Argument Parser

Copyright (C) 2018  Dominik Meyer

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/**
* @file example1.c
* @brief brief example on using YAAP
* @author Dominik Meyer <dmeyer@federationhq.de>
* @copyright 2018 by Dominik Meyer
*/

#include "../src/argparse.h"
#include <stdio.h>

/*
* @brief example command
*/
int test1(int argc, char **argv)
{
  printf("Test\n");
  return 0;
}


/*
* @brief main function for this example
*/
int main (int argc, char **argv)
// initialize the parsing context
{
  struct arg_parse_ctx *argparse_ctx = argparse_init();

  // create a command argument
  struct arg_parse_cmd test_cmd= {
    {0,1,0},                        // 1 = mandatory element
    0,
    "test",                         // command name
    "simple test",                  // command description
    &test1                          // if found call this function
  };

  /// add the argument command to context
  argparse_add_command(argparse_ctx, &test_cmd);

  int count;
  struct arg_int test_int = {
    {ARG_INT,0,0},
    'c',
    "count",
    "just a counting value",
    &count
    };
  /// add the argument int to context
  argparse_add_int(argparse_ctx, &test_int);

  /// parse the commandline
  int ret=argparse_parse(argparse_ctx, argc, argv);

  if (ret >= 0)
  {
    printf("Counter: %d\n", count);
  }

  /// free context
  argparse_free(argparse_ctx);

  return ret;

}
