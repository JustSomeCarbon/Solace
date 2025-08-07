#include <string.h>
#include <stdlib.h>
#include "lex.h"

/**
 * Create a new token from the values passed. The new token
 * is returned.
 */
Token create_token(int code_val, int line, char *tok_lit) {
  Token new_token;

  new_token.code = code_val;
  new_token.file_line = line;
  new_token.token_literal = (char*)malloc(strlen(tok_lit));
  if (new_token.token_literal == NULL) {
    printf("Error: unable to allocate memory for token %s\n\n", tok_lit);
    exit(1);
  }
  strcpy(new_token.token_literal, tok_lit);

  switch (code_val) {
    case INTLIT:
      new_token.token_value.i = atoi(tok_lit);
      break;
    case CHARLIT:
      new_token.token_value.c = tok_lit[0];
      break;
    case FLOATLIT:
      new_token.token_value.f = atof(tok_lit);
      break;
    case BOOLLIT:
      if (strcmp(tok_lit, "true") == 0) {
        new_token.token_value.i = 1;
      } else if (strcmp(tok_lit, "false") == 0) {
        new_token.token_value.i = 0;
      }
      break;
    default:
      new_token.token_value.i = 0;
      break;
  }

  return new_token;
}

/**
 * Takes the token stack and a new token and pushes the
 * new token onto the stack. New tokens are added to the
 * bottom of the current stack.
 */
void push_tokenstack(TokenStack *stack, Token* new_token) {
  stack->bottom->next = new_token;
  stack->bottom = stack->bottom->next;
  stack->size += 1;
}

/**
 * Takes a token stack and frees any allocated memory.
 */
void free_tokenstack(TokenStack *stack) {
  Token* current = stack->top;
  while (current->next != NULL) {
    free(current->token_literal);
    Token* empty_token = current;
    current = current->next;
    free(empty_token);
    stack->size -= 1;
  }

  if (stack->size != 0) {
    printf("Error: unable to free token stack, %d tokens unaccounted for\n\n", stack->size);
    free(stack);
    exit(1);
  }

  free(stack);
}
