#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SZ 50

// prototypes
void usage(char *);
void print_buff(char *, int);
int setup_buff(char *, char *, int);

// prototypes for functions to handle required functionality
int count_words(char *, int, int);
// add additional prototypes here
int reverse_string(char *, int, int);
int word_print(char *, int, int);
int str_replace(char *, int, int, char *, char *);

int setup_buff(char *buff, char *user_str, int len) {
  // TODO: #4:  Implement the setup buff as per the directions
  int buff_pos = 0;
  int str_poss = 0;
  char prev_char = '.';

  // Process input
  while (*(user_str + str_poss) != '\0') {

    if (buff_pos >= len) {
      // Fails if the user string is larger than the buffer size
      // Buffer overflow
      return 4;
    }

    char current = *(user_str + str_poss);

    // Checks for whitespace characters
    if (current == ' ' || current == '\t') {

      // Prevents duplicate whitespace
      if (prev_char != ' ') {
        *(buff + buff_pos) = ' ';
        buff_pos++;
      }

    } else {
      *(buff + buff_pos) = current;
      buff_pos++;
    }

    if (current == '\t')
      prev_char = ' ';
    else
      prev_char = current;

    str_poss++;
  }

  while (buff_pos < len) {
    *(buff + buff_pos) = '.';
    buff_pos++;
  }
  return str_poss;
}

void print_buff(char *buff, int len) {
  printf("Buffer:  ");
  for (int i = 0; i < len; i++) {
    putchar(*(buff + i));
  }
  putchar('\n');
}

void usage(char *exename) {
  printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);
}

int count_words(char *buff, int len, int str_len) {
  int wc = 0;
  bool word_start = false;

  for (int i = 0; i < str_len; i++) {
    char current = *(buff + i);

    // Check if we are inbetween a word
    if (!word_start) {

      // Check for whitespace and if we aren't then we're in a word
      if (current == ' ') {
        continue;
      }

      else {
        wc++;
        word_start = true;
      }

    } else if (current == ' ') {
      word_start = false;
    }
  }
  return 0;
}

int reverse_string(char *buff, int len, int str_len) {
  // Error checking
  if (str_len <= 0 || str_len > len) {
    return 3;
  }

  int start_idx = 0;
  int end_idx = str_len - 1;

  while (end_idx > start_idx) {
    char temp = *(buff + start_idx);

    // Swap
    *(buff + start_idx) = *(buff + end_idx);
    *(buff + end_idx) = temp;

    // Move indices
    start_idx++;
    end_idx--;
  }

  return 0;
}

int word_print(char *buff, int len, int str_len) {
  int wc = 0;
  int wlen = 0;
  bool word_start = false;

  printf("Word Print\n----------\n");

  for (int i = 0; i < str_len; i++) {
    char current = *(buff + i);

    if (!word_start && current != ' ') {
      // Starting a new word
      wc++;
      word_start = true;
      wlen = 1;
      printf("%d. %c", wc, current);

    } else if (word_start) {

      if (current != ' ') {
        // Continue current word
        printf("%c", current);
        wlen++;
      }

      // Check for word ending conditions
      if (current == ' ' || i == str_len - 1) {
        printf(" (%d)\n", wlen);
        word_start = false;
        wlen = 0;
      }
    }
  }

  return 0;
}

int str_replace(char *buff, int len, int str_len, char *target, char *replace) {

  // Check if replacement would exceed buffer
  int target_len = 0;
  int replace_len = 0;

  char *p_target = target;
  char *p_replace = replace;

  // Get the lengths of the target and replace strings
  while (*p_target != '\0') {
    target_len++;
    p_target++;
  }
  while (*p_replace != '\0') {
    replace_len++;
    p_replace++;
  }

  int new_len = str_len - target_len + replace_len;
  if (new_len > len) {
    return 4; // Buffer overflow
  }

  // Find substring
  char *match_pos = NULL;
  char *curr_pos = buff;

  while (*curr_pos && curr_pos < buff + str_len) {
    if (*curr_pos == *target) {
      char *buff_ptr = curr_pos;
      char *target_ptr = target;

      // Compare characters
      while (*buff_ptr && *target_ptr && *buff_ptr == *target_ptr) {
        buff_ptr++;
        target_ptr++;
      }

      // Found match if target_ptr reached end
      if (!*target_ptr) {
        match_pos = curr_pos;
        break;
      }
    }
    curr_pos++;
  }

  if (match_pos) {
    // Calculate shift amount
    int shift = replace_len - target_len;

    if (shift < 0) {
      // Replacement is shorter - shift left
      char *src = match_pos + target_len;
      char *dst = match_pos + replace_len;

      while (src < buff + str_len) {
        *dst++ = *src++;
      }
    } else if (shift > 0) {
      // Replacement is longer - shift right
      char *src = buff + str_len - 1;
      char *dst = src + shift;

      while (src >= match_pos + target_len) {
        *dst-- = *src--;
      }
    }

    // Copy replacement string
    char *p_rep = replace;
    curr_pos = match_pos;
    while (*p_rep) {
      *curr_pos++ = *p_rep++;
    }

    return 0;
  }

  return 1; // No replacement made
}

// ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS

int main(int argc, char *argv[]) {

  char *buff;         // placehoder for the internal buffer
  char *input_string; // holds the string provided by the user on cmd line
  char opt;           // used to capture user option from cmd line
  int rc;             // used for return codes
  int user_str_len;   // length of user supplied string

  // TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
  //       PLACE A COMMENT BLOCK HERE EXPLAINING
  if ((argc < 2) || (*argv[1] != '-')) {
    usage(argv[0]);
    exit(1);
  }

  opt = (char)*(argv[1] + 1); // get the option flag

  // handle the help flag and then exit normally
  if (opt == 'h') {
    usage(argv[0]);
    exit(0);
  }

  // WE NOW WILL HANDLE THE REQUIRED OPERATIONS

  // TODO:  #2 Document the purpose of the if statement below
  //       PLACE A COMMENT BLOCK HERE EXPLAINING
  if (argc < 3) {
    usage(argv[0]);
    exit(1);
  }

  input_string = argv[2]; // capture the user input string

  // TODO:  #3 Allocate space for the buffer using malloc and
  //           handle error if malloc fails by exiting with a
  //           return code of 99
  //  CODE GOES HERE FOR #3

  buff = malloc(BUFFER_SZ);
  if (buff == NULL) {
    exit(99);
  }

  user_str_len = setup_buff(buff, input_string, BUFFER_SZ); // see todos
  if (user_str_len < 0) {
    printf("Error setting up buffer, error = %d", user_str_len);
    exit(2);
  }

  switch (opt) {
  case 'c':
    rc = count_words(buff, BUFFER_SZ, user_str_len); // you need to implement
    if (rc < 0) {
      printf("Error counting words, rc = %d", rc);
      exit(2);
    }
    printf("Word Count: %d\n", rc);
    break;

  // TODO:  #5 Implement the other cases for 'r' and 'w' by extending
  //        the case statement options
  case 'r':
    rc = reverse_string(buff, BUFFER_SZ, user_str_len);
    if (rc < 0) {
      printf("Error reversing string, rc = %d", rc);
      exit(2);
    }
    printf("Reversed String: %d\n", rc);
    break;
  case 'w':
    rc = word_print(buff, BUFFER_SZ, user_str_len);
    if (rc < 0) {
      printf("Error printing string, rc = %d", rc);
      exit(2);
    }
    break;
  case 'x':
    if (argc != 5) {
      printf("Error replacing string, rc = %d", rc);
      exit(2);
    }
    rc = str_replace(buff, BUFFER_SZ, user_str_len, *(argv + 3), *(argv + 4));
    break;
  default:
    usage(argv[0]);
    exit(1);
  }

  // TODO:  #6 Dont forget to free your buffer before exiting
  print_buff(buff, BUFFER_SZ);
  free(buff);
  exit(0);
}

// TODO:  #7  Notice all of the helper functions provided in the
//           starter take both the buffer as well as the length.  Why
//           do you think providing both the pointer and the length
//           is a good practice, after all we know from main() that
//           the buff variable will have exactly 50 bytes?
//
// Having both allows us to enable proper bound checking to prevent buffer
// overflows and allows for potential easy modifications where we can change the
// buffer size
