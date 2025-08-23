#ifndef LEX_H
#define LEX_H
#include <stdio.h>

#define BUFFER_SIZE 2048


// contains the stored value of the token
typedef union TokenValue {
  int i;
  float f;
  char c;
} TokenValue;

// contains the token information
typedef struct Token {
  int code;               // token code
  int file_line;          // occurance line
  char* token_literal;    // literal token string
  TokenValue token_value; // stored token value
  struct Token* next;     // the next token in stack
} Token;

typedef struct TokenStack {
  Token* top;     // the top of the token stack
  Token* bottom;  // the bottom of the token stack
  int size;       // the size of the token stack
} TokenStack;

typedef struct BufReader {
  char buffer[BUFFER_SIZE]; // the read buffer
  size_t buf_size;          // the size of the living buffer 
  int index;                // the current index of the reader
} BufReader;

// functions defined for compiler lexing capabilities
TokenStack* lex_source_file(FILE* file_ptr);
Token* lex_word(BufReader* buf);
Token* lex_number(BufReader* buf);


// Token Utility functions
Token* create_token(int code_val, int line, char* tok_lit);
void push_tokenstack(TokenStack* stack, Token* new_token);
void free_tokenstack(TokenStack* stack);
void print_tokenstack(TokenStack* stack);


#endif
