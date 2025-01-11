#ifndef LEX_H
#define LEX_H
#include <stdio.h>

// functions defined for compiler lexing capabilities
void lex_source_file(FILE* file_ptr);

// contains the stored value of the token
typedef union TokenValue {
  int i;
  float f;
  char c;
  char* str;
} TokenValue;

// contains the token information
typedef struct Token {
  int code;               // token code
  int file_line;          // occurance line
  char* token_literal;    // literal token string
  TokenValue token_value; // stored token value
  struct Token* next;            // the next token in stack
} Token;

#endif
