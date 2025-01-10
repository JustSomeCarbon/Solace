#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
      if (!check_file_extension(argv[argument_index]))
      {
        printf("Error: file given is not a Solace source file\n\n");
        exit(1);
      }

      FILE* file_ptr = fopen(argv[argument_index], "r");
      printf("%s has been opened\n", argv[argument_index]);
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
