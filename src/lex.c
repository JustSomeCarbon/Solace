#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lex.h"
#include "representations.h"

void lex_loop(BufReader* buf, TokenStack* stack);
int populate_buffer(BufReader* bufr, FILE* file_ptr);
int handle_buffer_split(BufReader* bufr, int* lit_offset, int start_lit, int lit_len, char* lit);
int is_reserved_word(char* word);
void consume(BufReader* buf);
LiteralToken create_literal(char* val);

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
  if (populate_buffer(buf_reader, file_ptr) == 0) {
		free(stack);
    return NULL;
  }

  // initialize the file line and begin lexing loop
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
      if (!source_file || (populate_buffer(buf, source_file) == 0)) {
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
    } else if (cur_char == 32)
    {
      // space value
      buf->index++;
      continue;
    } else if (cur_char == 10)
    {
      // new line detected
      LiteralToken lit = create_literal("\n");
      Token* token = create_token(NEWLINE, file_line, lit);
      push_tokenstack(stack, token);
      file_line++;
    } else {
      // parse symbol
    }

    buf->index++;
  }
  return;
}


/**
 * Takes the buffer and lexes the user defined word found.
 * The result is a new token pointer of the parsed word.
 */
Token* lex_word(BufReader* buf)
{
  char lit[256];
  int lit_offset = 0;
  
  int start_word = buf->index;
  int cur_char = buf->buffer[buf->index];

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
 * The result is a new token pointer of the parsed number.
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
 * Takes the buffer and lexes the given symbol character(s).
 * The result is a new Token pointer of the parsed symbol.
 */
Token* lex_symbol(BufReader* buf) {
  char lit[2];
  int token_code = -1;

  int cur_sym = buf->buffer[buf->index];
  switch (cur_sym) {
    case 43:
      token_code = ADDITION;
      break;
    case 45:
      token_code = SUBTRACTION;
      break;
    case 42:
      token_code = MULTIPLICATION;
      break;
    case 47:
      token_code = DIVISION;
      break;
    case 38:
      token_code = ANDTAG;
      break;
    case 124:
      token_code = ORTAG;
      break;
    case 33:
      token_code = BANG;
      break;
    case 62:
      token_code = GREATERTHAN;
      break;
    case 60:
      token_code = LESSTHAN;
      break;
    case 61:
      token_code = EQUAL;
      break;
    case 40:
      token_code = PARENLEFT;
      break;
    case 41:
      token_code = PARENRIGHT;
      break;
    case 91:
      token_code = BRACKETLEFT;
      break;
    case 93:
      token_code = BRACKETRIGHT;
      break;
    case 123:
      token_code = BRACELEFT;
      break;
    case 125:
      token_code = BRACERIGHT;
      break;
    case 58:
      token_code = COLON;
      break;
    case 59:
      token_code = SEMICOLON;
      break;
    case 46:
      token_code = DOT;
      break;
    case 95:
      token_code = UNDERSCORE;
      break;
    default:
      printf("Error: unknown symbol %c on line %d\n", cur_sym, file_line);
      consume(buf);
  }

  if (token_code == -1) {
    return NULL;
  }

  lit[1] = '\0';
  lit[0] = buf->buffer[buf->index];
  Token* tok = create_token(token_code, file_line, lit);

  return tok;
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

/**
 * consumes the current word until a space or newline
 * character is found. The consumed word is discarded.
 */
void consume(BufReader* buf) {
  while (buf->buffer[buf->index] != ' ' || buf->buffer[buf->index] != '\n') {
    buf->index++;
    if (buf->index >= buf->buf_size) {
      if (!populate_buffer(buf, source_file)) {
        return;
      }
    }
  }
}

/**
 * takes a character string and 
 */
LiteralToken create_literal(char* val) {
  LiteralToken token_literal;
  token_literal.size = strlen(val);
  token_literal.value = (char*)malloc(token_literal.size*sizeof(char)+1);
  strncpy(token_literal.value, val, token_literal.size);
  token_literal.value[token_literal.size] = '\0';
  return token_literal;
}

LiteralToken construct_literal(LiteralToken lit, char* val) {
  if (lit.size == 0) {
    lit.size = strlen(val);
    lit.value = (char*)malloc(lit.size*sizeof(char)+1);
    strncpy(lit.value, val, lit.size);
    lit.value[lit.size] = '\0';
  } else {
    if (!realloc(lit.value, lit.size+strlen(val))) {
      printf("Error: unable to reallocate memory for Token Literal");
      printf("\t%s with:: %s", lit.value, val);
      // TERMINATE LEXING
    }
    strncpy(lit.value+lit.size, val, strlen(val));
    lit.size = lit.size + strlen(val);
  }
  return lit;
}
