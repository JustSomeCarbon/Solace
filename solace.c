#include <stdio.h>
#include <string.h>
#include "solace.h"

int main (int argc, char** argv)
{
  if (argc < 2)
  {
    basic_usage();
  }

  int argument_index = 1;
  for (; argument_index < argc; argument_index++)
  {
    // parse compiler flags
    if (strcmp(argv[argument_index], "-h") == 0)
    {
      basic_usage();
    } else if (strcmp(argv[argument_index], "-v") == 0)
    {
      version_information();
    } else 
    {
      // a file has been found
      if (argument_index+1 != argc) {
        printf("Error: given file should be the last argument\n");
        basic_usage();
        break;
      }
      // read and tokenize file
      FILE* file_ptr = fopen(argv[argument_index], "r");
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
