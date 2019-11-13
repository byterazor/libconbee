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
* @file argparse.c
* @brief file for all argparse functions
* @author Dominik Meyer <dmeyer@federationhq.de>
* @copyright 2018 by Dominik Meyer
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "argparse.h"


void argparse_usage(struct arg_parse_ctx *ctx, char *program); ///< forward declaration of function
int argparse_add_flag(struct arg_parse_ctx *ctx, struct arg_flag *flag); ///< forward declaration of function

/**
* @brief wrapper function for calling argparse_usage from the -h arg_flag command line argument
*
* @param ctx - the current yaap context
* @param user - some user specific data in this case the command line arguments
*
* @return 0 - everything is fine
*/
int argparse_help(void *ctx, void *user)
{
  argparse_usage(ctx, ((char **) user)[0]);
  return 0;
}

/**
* @brief initialize the argparse context
*
* This function has to be called at every start of the programm and before
* any other argparse functions are called!!!
*
* @return returns an initialized arg parse structure
*/
struct arg_parse_ctx * argparse_init()
{
  // allocate memory for the arg_parse_ctx structure
  struct arg_parse_ctx *ctx = (struct arg_parse_ctx *) calloc(sizeof(struct arg_parse_ctx),1);
  if (ctx == NULL)
  {
    return NULL;
  }

  // allocate memory for the arg_parse_cmd array
  ctx->arguments=(void **)calloc(sizeof(void *), ARGPARSE_INITIAL_COMMAND_NR);
  if (ctx->arguments == NULL)
  {
    free(ctx);
    return NULL;
  }

  // flag argument for -h and --help
  struct arg_flag *flag = (struct arg_flag *) calloc(sizeof(struct arg_flag),1);
  flag->base.type = ARG_FLAG;
  flag->short_flag='h';
  flag->long_flag="help";
  flag->description="print help message";
  flag->cb=&argparse_help;
  argparse_add_flag(ctx, flag);

  return ctx;
}

/**
* @brief free an arg_parse context (all memory will be freed)
*
* this function should be called at the end of the programm
*
* @param ctx - the arg_parse_ctx structure to free
*/
void argparse_free(struct arg_parse_ctx *ctx)
{
  int i;

  //search for -h flag argument and free it
  for(i=0; i< ctx->nr_arguments;i++)
  {
    if (to_argbase(ctx->arguments[i])->type == ARG_FLAG)
    {
      if (to_flag(ctx->arguments[i])->short_flag=='h')
      {
        free(ctx->arguments[i]);
      }
    }
  }
  // free the arg_parse_cmd array
  free(ctx->arguments);

  // free the context itself
  free(ctx);
}

/**
* @brief add an arg_parse_cmd to the context for later parsing
*
* @param ctx - the context the command should be added to
* @param cmd - the command to add to the context
*
* @return 0 = everything is fine, -1 error occured
*/
int argparse_add_command(struct arg_parse_ctx *ctx, struct arg_parse_cmd *cmd)
{
  if (ctx == NULL || cmd == NULL)
  {
    return -1;
  }

  cmd->base.type=ARG_CMD;

  // check if the array size has to be increased
  if (ctx->nr_arguments % ARGPARSE_INITIAL_COMMAND_NR == 0)
  {
    ctx->arguments= (void **) realloc(ctx->arguments, sizeof(void *)*(ctx->nr_arguments+ARGPARSE_INITIAL_COMMAND_NR));
  }

  // append the command to the array
  ctx->arguments[ctx->nr_arguments]=cmd;
  ctx->nr_arguments++;

  return 0;
}

/**
* @brief add an arg_str to the context for later parsing
*
* @param ctx - the context the command should be added to
* @param str - the arg_str to add
*
* @return 0 = everything is fine, -1 error occured
*/
int argparse_add_string(struct arg_parse_ctx *ctx, struct arg_str *str)
{
  if (ctx == NULL || str == NULL)
  {
    return -1;
  }

  str->base.type=ARG_STR;


  // check if the array size has to be increased
  if (ctx->nr_arguments % ARGPARSE_INITIAL_COMMAND_NR == 0)
  {
    ctx->arguments= (void **) realloc(ctx->arguments, sizeof(void *)*(ctx->nr_arguments+ARGPARSE_INITIAL_COMMAND_NR));
  }

  // append the command to the array
  ctx->arguments[ctx->nr_arguments]=str;
  ctx->nr_arguments++;

  return 0;
}

/**
* @brief add an arg_flag to the context for later parsing
*
* @param ctx - the context the command should be added to
* @param flag - the arg_flag to add
*
* @return 0 = everything is fine, -1 error occured
*/
int argparse_add_flag(struct arg_parse_ctx *ctx, struct arg_flag *flag)
{
  if (ctx == NULL || flag == NULL)
  {
    return -1;
  }

  flag->base.type=ARG_FLAG;


  // check if the array size has to be increased
  if (ctx->nr_arguments % ARGPARSE_INITIAL_COMMAND_NR == 0)
  {
    ctx->arguments= (void **) realloc(ctx->arguments, sizeof(void *)*(ctx->nr_arguments+ARGPARSE_INITIAL_COMMAND_NR));
  }

  // append the command to the array
  ctx->arguments[ctx->nr_arguments]=flag;
  ctx->nr_arguments++;

  return 0;
}

/**
* @brief add an arg_int to the context for later parsing
*
* @param ctx - the context the command should be added to
* @param integer - the arg_int to add
*
* @return 0 = everything is fine, -1 error occured
*/
int argparse_add_int(struct arg_parse_ctx *ctx, struct arg_int *integer)
{
  if (ctx == NULL || integer == NULL)
  {
    return -1;
  }

  integer->base.type=ARG_INT;


  // check if the array size has to be increased
  if (ctx->nr_arguments % ARGPARSE_INITIAL_COMMAND_NR == 0)
  {
    ctx->arguments= (void **) realloc(ctx->arguments, sizeof(void *)*(ctx->nr_arguments+ARGPARSE_INITIAL_COMMAND_NR));
  }

  // append the command to the array
  ctx->arguments[ctx->nr_arguments]=integer;
  ctx->nr_arguments++;

  return 0;
}

/**
* @brief internal function to print usage information, not exported to user
*
* @param ctx - the context for which to print the usage information
* @param program - the program name the application was called with
*
*/
void argparse_usage(struct arg_parse_ctx *ctx, char *program)
{
  int i=0;

  printf("usage: %s <command> [<args>]\n",program);
  printf("\n");

  printf("Available commands: \n");

  for(i=0; i<ctx->nr_arguments; i++)
  {
    switch (to_argbase(ctx->arguments[i])->type)
    {
      case ARG_CMD:
              printf("\t%20s\t\t%s\n", to_cmd(ctx->arguments[i])->command, to_cmd(ctx->arguments[i])->description);
              break;
      default:
        break;
    };
  }

  printf("\nAvailable Arguments: \n");
  for(i=0; i<ctx->nr_arguments; i++)
  {
    switch (to_argbase(ctx->arguments[i])->type)
    {
      case ARG_STR:
              printf("\t-%c <string>|--%s=<string>\t\t%s\n", to_str(ctx->arguments[i])->short_flag,
                                                               to_str(ctx->arguments[i])->long_flag,
                                                               to_str(ctx->arguments[i])->description);

              break;
      case ARG_FLAG:
            printf("\t-%c | --%s\t\t%s\n", to_flag(ctx->arguments[i])->short_flag,
                                            to_flag(ctx->arguments[i])->long_flag,
                                            to_flag(ctx->arguments[i])->description);
            break;
      case ARG_INT:
            printf("\t-%c <integer> | --%s=<integer>\t\t%s\n", to_flag(ctx->arguments[i])->short_flag,
                                           to_flag(ctx->arguments[i])->long_flag,
                                           to_flag(ctx->arguments[i])->description);
            break;
      default:
        break;
    };
  }

  printf("\n");

}

/**
* @brief internal function to print usage information, if an unknown command has been found
*
* @param ctx - the context for which to print the usage information
* @param program - the program name the application was called with
* @param command - the command found
*
*/
void argparse_unknown_command(struct arg_parse_ctx *ctx, char *program, char *command)
{
  printf("%s: \'%s\' is not a supported command.\n\n", program, command);
  argparse_usage(ctx, program);
}


/**
* @brief parse the given commandline
*
* @param ctx  - the arg parse context to use
* @param argc - the argument count of the commandline
* @param argv - the argument list of the commandline
*
* @return 0 - everything is fine
* @return <0 - parsing failed
*/
int argparse_parse(struct arg_parse_ctx *ctx, int argc, char **argv)
{
  int i=0;
  int r=0;
  int found=0;
  int newargc=argc;
  char **newargv=argv;

  if (ctx == NULL)
  {
    return -1;
  }

  // check if there are arguments to check for
  if (ctx->nr_arguments == 0)
  {
    return 0;
  }

  // check if one of the command line args matches a given argument type
  if (argc > 1)
  {
    for (i=1; i< argc; i++)
    {
      found=0;
      for(r=0; r < ctx->nr_arguments; r++)
      {
        // check if the argument is a command
        if (argv[i][0]!='-' && to_argbase(ctx->arguments[r])->type == ARG_CMD )
        {
          if (strcmp(to_cmd(ctx->arguments[r])->command,argv[i])==0)
          {
            found=1;
            newargc--;
            newargv=&argv[i];
            return to_cmd(ctx->arguments[r])->cb(newargc,newargv);
          }
        }
        // check if the argument is a string argument
        else if (argv[i][0]=='-' && to_argbase(ctx->arguments[r])->type == ARG_STR)
        {
          // check for long argument format or short
          if(argv[i][1]=='-')
          {
          }
          else
          {
            if(argv[i][1] == to_str(ctx->arguments[r])->short_flag)
            {
              found=1;
              if (i+1<argc)
              {
                strncpy(to_str(ctx->arguments[r])->value, argv[i+1], to_str(ctx->arguments[r])->maxchars);
                i++;
              }
              else
              {
                printf("value missing for -%c parameter\n\n",to_str(ctx->arguments[r])->short_flag);
                argparse_usage(ctx, argv[0]);
                return -1;
              }
              to_argbase(ctx->arguments[r])->set=1;
            }
          }
        }
        // check if the argument is a flag argument
        else if (argv[i][0]=='-' && to_argbase(ctx->arguments[r])->type == ARG_FLAG)
        {
          // check for long argument format or short
          if(argv[i][1]=='-')
          {
            if (strcmp(&argv[i][2],to_flag(ctx->arguments[r])->long_flag)==0)
            {
              to_flag(ctx->arguments[r])->base.set=1;
              found=1;
              if (to_flag(ctx->arguments[r])->cb != NULL)
              {
                return to_flag(ctx->arguments[r])->cb(ctx, argv);
              }
            }
          }
          else
          {
            if(argv[i][1] == to_flag(ctx->arguments[r])->short_flag)
            {
              found=1;
              to_flag(ctx->arguments[r])->base.set=1;
              if (to_flag(ctx->arguments[r])->cb != NULL)
              {
                return to_flag(ctx->arguments[r])->cb(ctx, argv);
              }

            }
          }
        }
        else if (argv[i][0]=='-' && to_argbase(ctx->arguments[r])->type == ARG_INT)
        {
          // check for long argument format or short
          if(argv[i][1]=='-')
          {
            char *tok=strtok(argv[i],"=");
            if (strcmp(tok+2,to_int(ctx->arguments[r])->long_flag)==0)
            {
              to_int(ctx->arguments[r])->base.set=1;
              tok=strtok(NULL,"=");
              found=1;
              if (to_int(ctx->arguments[r])->value != NULL)
              {
                *(to_int(ctx->arguments[r])->value)=(int) strtol(tok,NULL,10);
              }
            }
          }
          else
          {
            if(argv[i][1] == to_int(ctx->arguments[r])->short_flag)
            {
              found=1;
              to_int(ctx->arguments[r])->base.set=1;
              if (to_int(ctx->arguments[r])->value != NULL)
              {
                *(to_int(ctx->arguments[r])->value)=(int) strtol(argv[i+1],NULL,10);
              }
              i++;
            }
          }
        }
      }

      if (found == 0)
      {
          printf("%s is an unknown command/argument\n\n", argv[i]);
          argparse_usage(ctx, argv[0]);
          return -1;
      }
    }



  }

  // check if all mandatory parameters have been found
  for(i=0; i<ctx->nr_arguments; i++)
  {

    if (to_argbase(ctx->arguments[i])->mandatory==1 && to_argbase(ctx->arguments[i])->set==0)
    {
      printf("mandatory parameter missing ");
      switch (to_argbase(ctx->arguments[i])->type)
      {
        case ARG_STR:
          printf("-%c|--%s\n\n",to_str(ctx->arguments[i])->short_flag,to_str(ctx->arguments[i])->long_flag);
          break;

        default:
                printf("\n");
                break;
      };
      argparse_usage(ctx, argv[0]);
      return -1;
    }

  }


  return 0;
}
