#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dshlib.h"

/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */
int build_cmd_list(char *cmd_line, command_list_t *clist) {
  clist->num = 0;

  // Error checking
  if (strlen(cmd_line) == 0) {
    return WARN_NO_CMDS;
  }

  char *cmd_copy = strdup(cmd_line);
  char *pipe_token = strtok(cmd_copy, PIPE_STRING);

  // Process each command separated by pipes
  while (pipe_token != NULL) {
    if (clist->num >= CMD_MAX) {
      free(cmd_copy);
      return ERR_TOO_MANY_COMMANDS;
    }

    // Trim leading/trailing spaces
    while (*pipe_token == ' ')
      pipe_token++;
    char *end = pipe_token + strlen(pipe_token) - 1;
    while (end > pipe_token && *end == ' ')
      end--;
    *(end + 1) = '\0';

    // Split command and arguments
    char *space_pos = strchr(pipe_token, ' ');
    if (space_pos) {

      // Command has arguments
      int cmd_len = space_pos - pipe_token;
      strncpy(clist->commands[clist->num].exe, pipe_token, cmd_len);
      clist->commands[clist->num].exe[cmd_len] = '\0';

      // Copy arguments, skipping leading spaces
      space_pos++;
      while (*space_pos == ' ')
        space_pos++;
      strncpy(clist->commands[clist->num].args, space_pos, ARG_MAX - 1);
    } else {

      // Command without arguments
      strncpy(clist->commands[clist->num].exe, pipe_token, EXE_MAX - 1);
      clist->commands[clist->num].args[0] = '\0';
    }

    clist->num++;
    pipe_token = strtok(NULL, PIPE_STRING);
  }

  free(cmd_copy);
  return OK;
}
