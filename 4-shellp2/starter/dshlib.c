#include "dshlib.h"
#include <ctype.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts
 * the user for input.  Use the SH_PROMPT constant from dshlib.h and then use
 * fgets to accept user input.
 *
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 *
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 *
 *   Also, use the constants in the dshlib.h in this code.
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 *
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 *
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 *
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */

Built_In_Cmds match_command(const char *input) {
  if (input == NULL) {
    return BI_NOT_BI;
  }

  if (strcmp(input, EXIT_CMD) == 0) {
    return BI_CMD_EXIT;
  } else if (strcmp(input, "cd") == 0) {
    return BI_CMD_CD;
  } else if (strcmp(input, "dragon") == 0) {
    return BI_CMD_DRAGON;
  } else if (strcmp(input, "rc") == 0) {
    return BI_RC;
  }

  return BI_NOT_BI;
}

int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff) {
  if (cmd_line == NULL || cmd_buff == NULL) {
    return ERR_MEMORY;
  }

  // Trim leading spaces
  while (*cmd_line && isspace(*cmd_line)) {
    cmd_line++;
  }

  if (*cmd_line == '\0') {
    return WARN_NO_CMDS;
  }

  strcpy(cmd_buff->_cmd_buffer, cmd_line);

  // Parse arguments
  cmd_buff->argc = 0;
  char *token = cmd_buff->_cmd_buffer;
  bool in_quotes = false;
  bool in_token = false;
  char *arg_start = token;

  for (int i = 0; cmd_buff->_cmd_buffer[i] != '\0'; i++) {
    char c = cmd_buff->_cmd_buffer[i];

    if (c == '"') {
      in_quotes = !in_quotes;

      if (!in_token) {
        in_token = true;
        arg_start = &cmd_buff->_cmd_buffer[i + 1]; // Skip the quote
      } else if (!in_quotes) {
        // End of quoted token
        cmd_buff->_cmd_buffer[i] = '\0'; // Replace closing quote with null
        cmd_buff->argv[cmd_buff->argc++] = arg_start;
        in_token = false;
      }
    } else if (isspace(c) && !in_quotes) {
      if (in_token) {
        // End of token
        cmd_buff->_cmd_buffer[i] = '\0';
        cmd_buff->argv[cmd_buff->argc++] = arg_start;
        in_token = false;
      }
    } else if (!in_token) {
      // Start of new token
      in_token = true;
      arg_start = &cmd_buff->_cmd_buffer[i];
    }

    // Check if we've reached the maximum number of arguments
    if (cmd_buff->argc >= CMD_ARGV_MAX - 1) {
      break;
    }
  }

  // Handle the last token if it exists
  if (in_token) {
    cmd_buff->argv[cmd_buff->argc++] = arg_start;
  }

  // Null-terminate the argument list
  cmd_buff->argv[cmd_buff->argc] = NULL;

  return OK;
}
int alloc_cmd_buff(cmd_buff_t *cmd_buff) {
  if (cmd_buff == NULL) {
    return ERR_MEMORY;
  }

  cmd_buff->_cmd_buffer = (char *)malloc(SH_CMD_MAX);
  if (cmd_buff->_cmd_buffer == NULL) {
    return ERR_MEMORY;
  }

  cmd_buff->argc = 0;
  for (int i = 0; i < CMD_ARGV_MAX; i++) {
    cmd_buff->argv[i] = NULL;
  }

  return OK;
}

int free_cmd_buff(cmd_buff_t *cmd_buff) {
  if (cmd_buff == NULL) {
    return ERR_MEMORY;
  }

  if (cmd_buff->_cmd_buffer != NULL) {
    free(cmd_buff->_cmd_buffer);
    cmd_buff->_cmd_buffer = NULL;
  }

  cmd_buff->argc = 0;
  for (int i = 0; i < CMD_ARGV_MAX; i++) {
    cmd_buff->argv[i] = NULL;
  }

  return OK;
}

int clear_cmd_buff(cmd_buff_t *cmd_buff) {
  if (cmd_buff == NULL || cmd_buff->_cmd_buffer == NULL) {
    return ERR_MEMORY;
  }

  memset(cmd_buff->_cmd_buffer, 0, SH_CMD_MAX);
  cmd_buff->argc = 0;
  for (int i = 0; i < CMD_ARGV_MAX; i++) {
    cmd_buff->argv[i] = NULL;
  }

  return OK;
}

Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd) {
  if (cmd == NULL || cmd->argc == 0) {
    return BI_NOT_BI;
  }

  Built_In_Cmds type = match_command(cmd->argv[0]);

  switch (type) {
  case BI_CMD_EXIT:
    return BI_CMD_EXIT;

  case BI_CMD_CD:
    // Implement cd command
    if (cmd->argc > 1) {
      if (chdir(cmd->argv[1]) != 0) {
        perror("cd");
      }
    }
    return BI_EXECUTED;

  case BI_CMD_DRAGON:
    // Implement dragon command if needed
    return BI_EXECUTED;

  default:
    return BI_NOT_BI;
  }
}

int exec_local_cmd_loop() {
  char input_buffer[SH_CMD_MAX];
  cmd_buff_t cmd;
  int rc = 0;
  int last_return_code = 0;

  // Allocate memory for cmd buffer
  if (alloc_cmd_buff(&cmd) != OK) {
    fprintf(stderr, "Memory allocation error\n");
    return ERR_MEMORY;
  }

  while (1) {
    // Print prompt and get user input
    printf("%s", SH_PROMPT);
    if (fgets(input_buffer, SH_CMD_MAX, stdin) == NULL) {
      printf("\n");
      break;
    }

    // Remove trailing newline
    input_buffer[strcspn(input_buffer, "\n")] = '\0';

    // Clear command buffer for new command
    clear_cmd_buff(&cmd);

    // Parse input into command buffer
    rc = build_cmd_buff(input_buffer, &cmd);

    if (rc == WARN_NO_CMDS) {
      printf("%s", CMD_WARN_NO_CMD);
      continue;
    } else if (rc == ERR_TOO_MANY_COMMANDS) {
      printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
      continue;
    } else if (rc != OK) {
      // Handle other errors
      continue;
    }

    Built_In_Cmds built_in = match_command(cmd.argv[0]);

    if (built_in != BI_NOT_BI) {
      // Handle built-in commands
      built_in = exec_built_in_cmd(&cmd);

      if (built_in == BI_CMD_EXIT) {
        rc = OK_EXIT;
        break;
      } else if (built_in == BI_RC) {
        printf("%d\n", last_return_code);
        continue;
      }
    } else {
      // Handle external command with fork/exec
      pid_t pid = fork();

      if (pid < 0) {
        // Fork failed
        fprintf(stderr, "Fork failed\n");
        continue;
      } else if (pid == 0) {
        // Child process
        execvp(cmd.argv[0], cmd.argv);

        // If execvp returns, it failed
        fprintf(stderr, "Command not found in PATH\n");
        exit(2); // Using 2 for command not found
      } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
          last_return_code = WEXITSTATUS(status);
        } else {
          last_return_code = -1;
        }
      }
    }
  }

  free_cmd_buff(&cmd);

  return rc;
}
