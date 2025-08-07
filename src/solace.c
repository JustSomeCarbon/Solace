#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <stdlib.h>
#include "solace.h"
#include "lex.h"

short flag_lexer_print = 0;
int handle_source_file(FILE* fileptr, char* filename);


int main (int argc, char** argv)
{
  if (argc < 2)
  {
    basic_usage();
    exit(1);
  }

  FILE* file_ptr = NULL;
  int argument_index = 1;
  for (; argument_index < argc; argument_index++)
  {
    // parse compiler flags
    if (argv[argument_index][0] == '-') 
    {
      parse_flags(argv[argument_index]);
    } else 
    {
      if (argument_index+1 != argc) {
        printf("Error: given source file should be the last argument\n");
        basic_usage();
        break;
      }

      if (!check_file_extension(argv[argument_index]))
      {
        printf("Error: source files should end with extension .solx"); 
        exit(1);
      }

			if (!handle_source_file(file_ptr, argv[argument_index])) {
				printf("Error: can only handle a sinle source file\n\n");
				return -1;
			}
    }
  }

  if (file_ptr) {
	  TokenStack* stack = lex_source_file(file_ptr);
	  if (!stack) {
		  printf("Error: File %s empty\n", argv[argument_index]);
		  printf("\tRecomended: Define a 'Main' module in source file\n\n");
		  return -1;
	  }
    
    if (flag_lexer_print) {
      print_tokenstack(stack);
    }

    free_tokenstack(stack);
    fclose(file_ptr);
  }

  return 0;
}

int handle_source_file(FILE* fileptr, char* filename) {
  if (!fileptr) {
    fileptr = fopen(filename, "r");
    if (!fileptr) {
      printf("Error: unable to open sourcefile: %s\n\n", filename);
      exit(-1);
    }
    return 0;
  }
  return 1;
}

/**
 * basic_usage() -
 * Prints the basic usage of the compiler program
 * to the terminal.
 */
void basic_usage()
{
  printf("Solace Compiler basic usage:\n");
  printf("\t>$ ./anvl [flags] [filename]\n\n");
}

/**
 * version_information() -
 * Prints the version information of the compiler program
 * to the terminal.
 */
void version_information()
{
  printf("Solace Compiler\n");
  printf("===================\n");
  printf("Ver:: 0.0.1\n\n");
}

/**
 * parse_flags -
 * Takes the given flags as a char* and parses each out.
 */
void parse_flags(char* flags) {
	int index = 1;
	while (flags[index]) {
		// walk through flags and populate flag variables
		switch (flags[index]) {
			case 'h':
				basic_usage();
				exit(0);
			case 'v':
				version_information();
				exit(0);
      case 'x':
        flag_lexer_print = 1;
        break;
			default:
				printf("Error: unknown flag %c\n", flags[index]);
		}
		index++;
	}
}

/**
 * check_file_extension -
 * Takes a file name as a char pointer and determines if the
 * file is a Solace source file. returns 1 if it is, 0
 * if not.
 */
int check_file_extension(char* file_name)
{
  char* extension = strrchr(file_name, '.');
  if (!extension)
  {
    return 0;
  }
  if (strcmp(extension+1, "solx") != 0)
  {
    return 0;
  }
  return 1;
}
