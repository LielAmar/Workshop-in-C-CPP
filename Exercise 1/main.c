#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cipher.h"
#include "tests.h"

#define BUFFER_LENGTH 1024
#define ARG_COUNT_TEST 2
#define ARG_COUNT_CMD 5

#define STRTOL_BASE 10

int handle_test_input (char *argv[]);
int handle_command_input (char *argv[]);
int run_command (char *command, int k, char *input_path, char *output_path);

int run_tests (void);

int is_integer (const char *str);
int parse_integer (const char *str);

int main (int argc, char *argv[])
{
  if (argc != ARG_COUNT_TEST && argc != ARG_COUNT_CMD)
  {
    fprintf (stderr, "The program receives 1 or 4 arguments only.\n");
    return EXIT_FAILURE;
  }

  return argc == ARG_COUNT_TEST ? handle_test_input (argv)
                                : handle_command_input (argv);
}

/**
 * @brief Handles the program's command mode.
 *
 * @param argv the program's arguments
 * @return EXIT_SUCCESS if the tests were successful, EXIT_FAILURE otherwise
 */
int handle_command_input (char *argv[])
{
  char *command = argv[1];
  if (strcmp (command, "encode") != 0 && strcmp (command, "decode") != 0)
  {
    fprintf (stderr, "The given command is invalid.\n");
    return EXIT_FAILURE;
  }

  // Setting endptr & errno to detect whether the returned value of strtol is
  // a valid integer, or whether the action failed.
  char *endptr = NULL;
  errno = 0;

  int k = (int) strtol (argv[2], &endptr, STRTOL_BASE);
  if (errno != 0 || !argv[2] || *endptr)
  {
    fprintf (stderr, "The given shift value is invalid.\n");
    return EXIT_FAILURE;
  }

  char *input_path = argv[3];
  char *output_path = argv[4];

  return run_command (command, k, input_path, output_path);
}

/**
 * @brief Runs the actual encryption/decryption command.
 *
 * @param command command to run
 * @param k key to use
 * @param input_path path of the input text
 * @param output_path path for the output text
 * @return EXIT_SUCCESS if the command was successful, EXIT_FAILURE otherwise
 */
int run_command (char *command, int k, char *input_path, char *output_path)
{
  FILE *input_file = fopen (input_path, "r");
  if (input_file == NULL)
  {
    fprintf (stderr, "The given file is invalid.\n");
    return EXIT_FAILURE;
  }

  FILE *output_file = fopen (output_path, "w");
  if (output_file == NULL)
  {
    fclose (input_file);
    fprintf (stderr, "The given file is invalid.\n");
    return EXIT_FAILURE;
  }

  char input[BUFFER_LENGTH];
  while (fgets (input, sizeof (input), input_file) != NULL)
  {
    // Encoding/Decoding the line
    strcmp (command, "encode") == 0 ? encode (input, k) : decode (input, k);

    // Writing the line to the output file
    fputs (input, output_file);
  }

  fclose (input_file);
  fclose (output_file);

  return EXIT_SUCCESS;
}

/**
 * @brief Handles the program's test mode.
 *
 * @param argv the program's arguments
 * @return EXIT_SUCCESS if the tests were successful, EXIT_FAILURE otherwise
 */
int handle_test_input (char *argv[])
{
  if (strcmp (argv[1], "test") == 0)
  {
    return run_tests ();
  }

  fprintf (stderr, "Usage: cipher test\n");
  return EXIT_FAILURE;
}

/**
 * @brief Runs all tests for the test mode.
 *
 * @return EXIT_SUCCESS if the tests were successful, EXIT_FAILURE otherwise
 */
int run_tests (void)
{
  if (test_encode_non_cyclic_lower_case_positive_k () != 0)
  {
    return EXIT_FAILURE;
  }

  if (test_encode_cyclic_lower_case_special_char_positive_k () != 0)
  {
    return EXIT_FAILURE;
  }

  if (test_encode_non_cyclic_lower_case_special_char_negative_k () != 0)
  {
    return EXIT_FAILURE;
  }

  if (test_encode_cyclic_lower_case_negative_k () != 0)
  {
    return EXIT_FAILURE;
  }

  if (test_encode_cyclic_upper_case_positive_k () != 0)
  {
    return EXIT_FAILURE;
  }

  if (test_decode_non_cyclic_lower_case_positive_k () != 0)
  {
    return EXIT_FAILURE;
  }

  if (test_decode_cyclic_lower_case_special_char_positive_k () != 0)
  {
    return EXIT_FAILURE;
  }

  if (test_decode_non_cyclic_lower_case_special_char_negative_k () != 0)
  {
    return EXIT_FAILURE;
  }

  if (test_decode_cyclic_lower_case_negative_k () != 0)
  {
    return EXIT_FAILURE;
  }

  if (test_decode_cyclic_upper_case_positive_k () != 0)
  {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}