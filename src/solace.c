#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <stdlib.h>
#include "solace.h"
#include "lex.h"


int main (int argc, char** argv)
{
  if (argc < 2)
  {
    basic_usage();
    exit(1);
  }

  int argument_index = 1;
  for (; argument_index < argc; argument_index++)
  {
    // parse compiler flags
    if (argv[argument_index][0] == '-') 
    {
      parse_flags(argv[argument_index]);
    } else 
    {
      // Source file
      if (argument_index+1 != argc) {
        printf("Error: given file should be the last argument\n");
        basic_usage();
        break;
      }
      // check file extension
      if (!check_file_extension(argv[argument_index]))
      {
        printf("Error: file given is not a Solace source file\n\n");
        exit(1);
      }

			// Lex; Tokenize
      FILE* file_ptr = fopen(argv[argument_index], "r");
			if (!file_ptr) {
				printf("Error: could not open file %s\n\n", argv[argument_index]);
				return -1;
			}

			// DEBUG::
      printf("%s has been opened\n", argv[argument_index]);

			// TODO:: call lexer
			TokenStack* stack = lex_source_file(file_ptr);
			if (!stack) {
				printf("Error: File %s empty\n", argv[argument_index]);
				printf("\tRecomended: Define a 'Main' module in source file\n\n");
				return -1;
			}

      fclose(file_ptr);
      printf("%s has been closed\n", argv[argument_index]);
    }
  }

  return 0;
}

/**
 * basic_usage() -
 * Prints the basic usage of the compiler program
 * to the terminal.
 */
void basic_usage()
{
  printf("Solace Compiler basic usage:\n");
  printf("\t>$ ./solc [flags] [filename]\n\n");
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
  if (strcmp(extension+1, "solc") != 0)
  {
    return 0;
  }
  return 1;
}
