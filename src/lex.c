#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lex.h"
#include "representations.h"

void lex_loop(BufReader* buf, TokenStack* stack);
int populate_buffer(BufReader* bufr, FILE* file_ptr);
int handle_buffer_split(BufReader* bufr, int* lit_offset, int start_lit, int lit_len, char* lit);
int is_reserved_word(char* word);

FILE* source_file = NULL;
int file_line = 0;

/**
 * Takes the source file and performs lexing functions over the source.
 * A token stack is returned.
 */
TokenStack* lex_source_file(FILE* file_ptr)
{
  source_file = file_ptr;
  TokenStack* stack = (TokenStack*)malloc(sizeof(TokenStack));
	// Create and populate the buffer reader
  BufReader buffer; // TODO: cleanup??
  BufReader* buf_reader = &buffer;
  if (populate_buffer(buf_reader, source_file) == 0) {
		free(stack);
    return NULL;
  }
  file_line = 1;
  
  lex_loop(buf_reader, stack);
  return stack;
}


/*
 * Loops through the buffer and lexes tokens.
 */
void lex_loop(BufReader* buf, TokenStack* stack)
{
  printf("in lex loop\n"); // REMOVE
  while (1) {
    // check buffer index
    if (buf->index >= buf->buf_size) {
      if (populate_buffer(buf, source_file) == 0) {
        break;
      }
    }

    int cur_char = buf->buffer[buf->index];
    if ((cur_char >= 65 && cur_char <= 90) || (cur_char >= 97 && cur_char <= 122) || (cur_char == 95))
    {
      // user defined word
      push_tokenstack(stack, lex_word(buf));

    } else if (cur_char >= 48 && cur_char <= 57)
    {
      // number literal value
      push_tokenstack(stack, lex_number(buf));
    }

    buf->index++;
  }
  return;
}


/**
 * Takes the buffer and lexes the user defined word found.
 * The result is a new token pointer that is then returned.
 */
Token* lex_word(BufReader* buf)
{
  char lit[256];
  int lit_offset = 0;
  
  int start_word = buf->index;
  int cur_char = buf->buffer[buf->index];

  // walk through the word
  while((cur_char >= 65 && cur_char <= 90) || (cur_char >= 97 && cur_char <= 122) || (cur_char == 95)) {
    buf->index++;
    // if word is split between buffer reads
    if (buf->index >= buf->buf_size) {
      int word_length = buf->index - start_word;
      if (handle_buffer_split(buf, &lit_offset, start_word, word_length, lit) == 0) {
        break;
      }
      start_word = buf->index;
    }

    cur_char = buf->buffer[buf->index];
  }

  // get the token literal value
  int word_length = buf->index - start_word;
  if (word_length > 255) {
    printf("Error: word found at %d exceeds memory limit of 255 characters\n\n", file_line);
    word_length = 255;
  }
  if (word_length != 0) {
    strncpy(lit + lit_offset, buf->buffer + start_word, word_length);
  }
  lit[word_length] = '\0';

  int token_code = is_reserved_word(lit);
  Token* token = create_token(token_code, file_line, lit);
  return token;
}

/**
 * Takes the buffer and lexes the given number literal.
 * The result is a new token pointer that is then returned.
 */
Token* lex_number(BufReader* buf) {
  char lit[255];
  int lit_offset = 0;
  int is_real = 0;

  int start_num = buf->index;
  int cur_num = buf->buffer[buf->index];

  while (((cur_num >= 48) || (cur_num <= 57)) || cur_num == 46) {
    buf->index++;
    if (buf->index >= buf->buf_size) {
      int num_length = buf->index - start_num;
      if (handle_buffer_split(buf, &lit_offset, start_num, num_length, lit) == 0) {
        break;
      }
      start_num = buf->index;
    }

    if (cur_num == 46 && !is_real) {
      is_real = 1;
    } else if (cur_num == 46 && is_real) {
      // THROW ERROR
    }
    cur_num = buf->buffer[buf->index];
  }

  int num_length = buf->index - start_num;
  if (num_length > 255) {
    printf("Error: word found at %d exceeds memory limit of 255 characters\n\n", file_line);
    num_length = 255;
  }
  if (num_length != 0) {
    strncpy(lit + lit_offset, buf->buffer + start_num, num_length);
  }
  lit[num_length] = '\0';
  int token_code = 1017;
  if (is_real) {
    token_code = 1019;
  }
  Token* token = create_token(token_code, file_line, lit);

  return token;
}


/**
 * Takes the BufReader and populates it with new values
 * read from the given file pointer. If nothing is read
 * 0 is returned, 1 otherwise.
 */
int populate_buffer(BufReader* bufr, FILE* file_ptr) {
  bufr->index = 0;
  bufr->buf_size = fread(bufr->buffer, 1, BUFFER_SIZE, file_ptr);
  if (bufr->buf_size == 0) {
    return 0;
  }
  return 1;
}

/**
 * Handles a buffer split in between a token read. An integer values
 * is returned based on whether the operation was successful or not
 * 1 if the buffer is populated, 0 if nothing is read or EOF.
 */
int handle_buffer_split(BufReader* bufr, int* lit_offset,int start_lit, int lit_len, char* lit) {
  if (*lit_offset < 255) {
        if (lit_len + *lit_offset <= 255) {
          strncpy(lit + *lit_offset, bufr->buffer + start_lit, lit_len);
        } else {
          strncpy(lit + *lit_offset, bufr->buffer + start_lit, 255 - (*lit_offset + lit_len));
        }
      }
      *lit_offset += lit_len;
  return populate_buffer(bufr, source_file);
}
/**
 * Determines if the given word is a reserved word. If
 * so, the associated tokenCode is returned, otherwise
 * the USERWORD code is returned.
 * TODO :: Migrate to a hash table, O(1)
 *    implement better data relation, word -> enum value
 */
int is_reserved_word(char* word) {
  unsigned int size = sizeof(reserved_words) / sizeof(reserved_words[0]);
  for(int index = 0; index < size; index++) {
    if (strcmp(word, reserved_words[index]) == 0) {
      return 1001 + index;
    }
  }

  return USERWORD;
}
