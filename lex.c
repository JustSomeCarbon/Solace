#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lex.h"
#include "representations.h"

void lex_loop(BufReader* buf, TokenStack* stack);
int populate_buffer(BufReader* bufr, FILE* file_ptr);

FILE* source_file;
int file_line;

/**
 * Takes the source file and performs lexing functions over the source.
 * A token stack is returned.
 */
TokenStack* lex_source_file(FILE* file_ptr)
{
  source_file = file_ptr;
  TokenStack* stack = (TokenStack*)malloc(sizeof(TokenStack));
  BufReader buffer;
  BufReader* buf_reader = &buffer;
  if (populate_buffer(buf_reader, source_file) == 0) {
    printf("Error: file has no contents\n\n");
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
  while (1) {
    // check buffer index
    if (buf->index >= buf->buf_size) {
      if (populate_buffer(buf, source_file) == 0) {
        break;
      }
    }

    int cur_char = buf->buffer[buf->index];
    if ((cur_char >= 65 && cur_char <= 90) || (cur_char >= 97 && cur_char <= 122))
    {
      // user defined word
      push_tokenstack(stack, lex_word(buf));

    } else if (cur_char >= 48 && cur_char <= 57)
    {
      // number literal value
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
  while((cur_char >= 65 && cur_char <= 90) || (cur_char >= 97 && cur_char <= 122)) {
    buf->index++;
    if (buf->index >= buf->buf_size) {
      // word falls outside of current buffer read.
      // cast arcane fuckery to handle it
      int word_length = buf->index - start_word;
      strncpy(lit + lit_offset, buf->buffer + start_word, word_length);
      lit_offset = word_length;
      if (populate_buffer(buf, source_file) == 0) {
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
  if (start_word != buf->index) {
    strncpy(lit + lit_offset, buf->buffer + start_word, word_length);
  }

  Token token = create_token(USERWORD, file_line, lit);
  Token* token_ptr = &token;
  return token_ptr;
}

Token* lex_number(BufReader* buffer);


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
