#ifndef __ARGPARSE_H__
#define __ARGPARSE_H__
/*
argparse functions

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
* @file argparse.h
* @brief header file for all argparse functions
* @author Dominik Meyer <dmeyer@federationhq.de>
* @copyright 2018 by Dominik Meyer
*/


/** @mainpage
*
* \section intro_sec Introduction
* YAAP is a simple command line argument parser for C.
* It uses functions and structures to add arguments to the parsing context and
* parse the command line.
* \section Features
*  - git like command support (git commit)
*  - short and long flag support ( -t YAAP or --title=YAAP)
*  - string argument support ( -t YAAP or -- title=YAAP)
*  - simple flags (-v, -V, -h, --help)
*  - integer arguments (-c 10)
*
* \section coming
*
*  - hex arguments ( -c 0x5A)
*
* \section Repository
*
* https://github.com/byterazor/yaap
*
* \section Installation
* YAAP is intented to be copied into your project directory. Different approaches are
* available:
*  - simpe copy argparse.h and argparse.c into your source directory
*  - add the github repository as a submodule
*  - add the github repository as a subtree
*  - add the github repository as a subrepo
*
* In all cases you have to make sure that argparse.c is compiled into an object file
* and is appended to your link stage.
*
* \section Functions
* All available functions can be found in argparse.c.
*
*  @example example1.c
*/




#define ARGPARSE_INITIAL_COMMAND_NR 5     ///< initial size for the command array

#define to_argbase(ptr) ((struct arg_base *) ptr)   ///< convert a pointer to an arg_base structure pointer
#define to_cmd(ptr) ((struct arg_parse_cmd *)ptr)   ///< convert a pointer to an arg_parse_cmd structure pointer
#define to_str(ptr) ((struct arg_str *)ptr)         ///< convert a pointer to an arg_str structure pointer
#define to_flag(ptr) ((struct arg_flag *) ptr)      ///< convert a pointer to an arg_flag structure pointer
#define to_int(ptr) ((struct arg_int *) ptr)        ///< convert a pointer to an arg_int structure pointer

/**
* @brief supported argument types
*/
enum argtypes {
  ARG_CMD,        ///< a command argument like git uses it (git commit)
  ARG_FLAG,       ///< a simple flag like -v without additional parameter
  ARG_STR,        ///< an argument with a string parameter (-t hallo, --title=hallo)
  ARG_INT         ///< an argument with a integer parameter (-s 10, --size=10)
};

/**
* @brief base structure for a command line argument
*/
struct arg_base
{
  enum argtypes type;             ///< type of the argument
  int mandatory;                  ///< is the argument mandatory
  int set;                        ///< has the argument been set
};

/**
* @brief structure representing a command
*/
struct arg_parse_cmd
{
  struct arg_base base;             ///< base of the command line argument
  int no_command;                   ///< if this is 1, the callback will be called if no command is given
  const char *command;              ///< the name of the command on the commandline, no spaces or special chars allowed, only ascii
  const char *description;          ///< short description of the command
  int (*cb)(int argc, char **argv); ///< function to call if the command is found on commandline
};

/**
* @brief structure representing a string command line argument
*/
struct arg_str {
  struct arg_base base;           ///< base of the command line argument
  char short_flag;          ///< one char flag identifying the argument, ignored if NULL
  const char *long_flag;          ///< multi char flag identifying the argument, ignore if NULL
  char *value;                    ///< memory already allocated for the string parameter
  int maxchars;                   ///< the maximum number of chars for the string parameters
  const char *description;        ///< short description of the argument
};

/**
*  @brief structure representing a simple flag command line argument
*/
struct arg_flag {
  struct arg_base base;           ///< base of the command line argument
  char short_flag;          ///< one char flag identifying the argument, ignored if NULL
  const char *long_flag;          ///< multi char flag identifying the argument, ignore if NULL
  const char *description;        ///< short description of the argument
  int (*cb)(void *ctx, void *userdata);  ///< callback called if argument is found
};

/**
*  @brief structure representing a simple int command line argument
*/
struct arg_int {
  struct arg_base base;           ///< base of the command line argument
  char short_flag;                ///< one char flag identifying the argument, ignored if NULL
  const char *long_flag;          ///< multi char flag identifying the argument, ignore if NULL
  const char *description;        ///< short description of the argument
  int *value;                     ///< the value of the option
};



/**
* @brief structure for the argparse context, holding argparse specific data structures
*/
struct arg_parse_ctx
{
  void **arguments;                  ///< array for all argparse elementsdd
  int nr_arguments;                  ///< how many arguments are currently registered
};


struct arg_parse_ctx * argparse_init();                                           ///< initialize parsing context
void argparse_free(struct arg_parse_ctx *ctx);                                    ///< free the parsing context at program end
int argparse_add_command(struct arg_parse_ctx *ctx, struct arg_parse_cmd *cmd);   ///< add a commandline command to the context
int argparse_add_string(struct arg_parse_ctx *ctx, struct arg_str *str);          ///< add a string argument to the context
int argparse_add_flag(struct arg_parse_ctx *ctx, struct arg_flag *flag);          ///< add a flag argument to the context
int argparse_add_int(struct arg_parse_ctx *ctx, struct arg_int *integer);          ///< add a integer argument to the context
int argparse_parse(struct arg_parse_ctx *ctx,int argc, char **argv);              ///< parse the given commandline in the context

#endif
