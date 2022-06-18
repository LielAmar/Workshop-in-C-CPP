#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sort_bus_lines.h"
#include "test_bus_lines.h"

#define BUFFER_LENGTH 60
#define ARG_LENGTH 20

#define MAX_LINE_NUMBER 999
#define MIN_LINE_NUMER 1
#define MAX_DISTANCE 1000
#define MIN_DISTANCE 0
#define MAX_DURATION 100
#define MIN_DURATION 10

const char *test_result_format = "TEST %d %s: %s\n";

bool check_arguments (int argc, char *argv[]);
void get_number_of_bus_lines_input (int *number_of_bus_lines);
bool get_bus_lines_input (BusLine **start, BusLine **end, int amount);
void get_bus_line_input (BusLine *busLine);

bool get_line_info (char *line_info_raw);
bool check_line_data (char *line_info_raw, int *line_number, int *distance,
                      int *duration);

void run_bubble_sort (BusLine *start, BusLine *end);
void run_quick_sort (BusLine *start, BusLine *end);
void run_tests (BusLine *start, BusLine *end);

/**
 * The main function to run the sort_lines program.
 *
 * @brief
 * - use '<program_name> test' to run the application's test mode
 * - use '<program_name> bubble' to run the bubble sort mode
 * - use '<program_name> quick' to run the quick sort mode
 *
 * @param argc number of arguments inserted
 * @param argv array of arguments inserted
 * @return 0 if the program ends successfully, 1 otherwise
 */
int main (int argc, char *argv[])
{
  if (!check_arguments (argc, argv))
  {
    return EXIT_FAILURE;
  }

  int number_of_bus_lines = 0;
  get_number_of_bus_lines_input (&number_of_bus_lines);

  BusLine start;
  BusLine *start_p = &start, *end_p;
  if (!get_bus_lines_input (&start_p, &end_p, number_of_bus_lines))
  {
    free (start_p);
    return EXIT_FAILURE;
  }

  if (strcmp (argv[1], "bubble") == 0)
  {
    run_bubble_sort (start_p, end_p);
  }
  else if (strcmp (argv[1], "quick") == 0)
  {
    run_quick_sort (start_p, end_p);
  }
  else if (strcmp (argv[1], "test") == 0)
  {
    run_tests (start_p, end_p);
  }

  free (start_p);

  return EXIT_SUCCESS;
}

/**
 * Checks if the given arguments are valid for the program
 *
 * @param argc number of arguments given
 * @param argv given arguments values
 * @return true if arguments are valid, 0 otherwise
 */
bool check_arguments (int argc, char *argv[])
{
  if (argc != 2
      || (strcmp (argv[1], "bubble") != 0 && strcmp (argv[1], "quick") != 0
          && strcmp (argv[1], "test") != 0))
  {
    printf ("USAGE: please execute the program with a signle argument: "
            "<test/bubble/quick>\n");
    return false;
  }

  return true;
}

/**
 * Asks the user for the number of bus lines to be sorted.
 * It keeps on asking the user for an input until the received input is valid.
 *
 * @param number_of_bus_lines a pointer to the int that should store the
 *                            user's input.
 */
void get_number_of_bus_lines_input (int *number_of_bus_lines)
{
  do
  {
    printf ("Enter number of lines. Then enter\n");

    char number_of_lines_raw[BUFFER_LENGTH] = { 0 };
    char *fgets_res = fgets (&number_of_lines_raw[0], BUFFER_LENGTH, stdin);

    if (fgets_res == NULL)
    {
      printf ("ERROR: Number of lines should be a positive integer\n");
      continue;
    }

    int result = sscanf (&number_of_lines_raw[0], "%d", number_of_bus_lines);
    if (result != 1 || result == EOF || (*number_of_bus_lines) <= 0)
    {
      printf ("ERROR: Number of lines should be a positive integer\n");
    }
  } while ((*number_of_bus_lines) <= 0);
}

/**
 * Asks the user for ${amount} bus lines data to insert to the given array.
 *
 * @param start a pointer to the pointer of the first element in the array
 * @param end a pointer to the pointer of the last element in the array
 * @param amount the number of bus lines to be inserted
 * @return true if was able to allocate memory, false otherwise
 */
bool get_bus_lines_input (BusLine **start, BusLine **end, int amount)
{
  (*start) = (BusLine *) malloc (sizeof (BusLine) * amount);

  if ((*start) == NULL)
  {
    return false;
  }

  (*end) = (*start) + amount;

  for (int i = 0; i < amount; i++)
  {
    get_bus_line_input ((*start) + i);
  }

  return true;
}

/**
 * Asks the user for a single bus line data to insert to the given element.
 *
 * @param busLine a pointer to the bus line to update its value to the given
 * data
 */
void get_bus_line_input (BusLine *busLine)
{
  int line_number = 0, distance = 0, duration = 0;

  do
  {
    printf ("Enter line info. Then enter\n");

    char line_info_raw[BUFFER_LENGTH] = { 0 };
    if (!get_line_info (&line_info_raw[0]))
    {
      continue;
    }

    if (!check_line_data (&line_info_raw[0], &line_number, &distance,
                          &duration))
    {
      continue;
    }
  } while (line_number < MIN_LINE_NUMER || line_number > MAX_LINE_NUMBER
           || distance < MIN_DISTANCE || distance > MAX_DISTANCE
           || duration < MIN_DURATION || duration > MAX_DURATION);

  (*busLine).line_number = line_number;
  (*busLine).distance = distance;
  (*busLine).duration = duration;
}

bool get_line_info (char *line_info_raw)
{
  char *fgets_res = fgets (line_info_raw, BUFFER_LENGTH, stdin);

  if (fgets_res == NULL)
  {
    printf ("ERROR: line_number should be an integer between %d and %d "
            "(includes)\n",
            MIN_LINE_NUMER, MAX_LINE_NUMBER);
    return false;
  }

  return true;
}

bool check_line_data (char *line_info_raw, int *line_number, int *distance,
                      int *duration)
{
  int result
      = sscanf (line_info_raw, "%d,%d,%d", line_number, distance, duration);

  if (result == EOF || (*line_number) < MIN_LINE_NUMER
      || (*line_number) > MAX_LINE_NUMBER)
  {
    printf ("ERROR: line_number should be an integer between %d and %d "
            "(includes)\n",
            MIN_LINE_NUMER, MAX_LINE_NUMBER);
    return false;
  }
  else if ((*distance) < MIN_DISTANCE || (*distance) > MAX_DISTANCE)
  {
    printf ("ERROR: distance should be an integer between %d and %d "
            "(includes)\n",
            MIN_DISTANCE, MAX_DISTANCE);
    return false;
  }
  else if ((*duration) < MIN_DURATION || (*duration) > MAX_DURATION)
  {
    printf ("ERROR: duration should be an integer between %d and %d "
            "(includes)\n",
            MIN_DURATION, MAX_DURATION);
    return false;
  }

  return true;
}

/**
 * Runs the bubble-sort mode of the application - sorts the given array by
 * distance.
 *
 * @param start start of the array to sort
 * @param end end of the array to sort
 */
void run_bubble_sort (BusLine *start, BusLine *end)
{
  bubble_sort (start, end);
  for (int i = 0; i < get_number_of_elements (start, end); i++)
  {
    printf ("%d,%d,%d\n", (*(start + i)).line_number, (*(start + i)).distance,
            (*(start + i)).duration);
  }
}

/**
 * Runs the quick-sort mode of the application - sorts the given array by
 * duration.
 *
 * @param start start of the array to sort
 * @param end end of the array to sort
 */
void run_quick_sort (BusLine *start, BusLine *end)
{
  quick_sort (start, end);
  for (int i = 0; i < get_number_of_elements (start, end); i++)
  {
    printf ("%d,%d,%d\n", (*(start + i)).line_number, (*(start + i)).distance,
            (*(start + i)).duration);
  }
}

/**
 * Runs the test mode of the application - runs 4 different tests
 *
 * @param start start of the array to test
 * @param end end of the array to test
 */
void run_tests (BusLine *start, BusLine *end)
{
  bool test_res = test_bubble_sort (start, end);
  printf (test_result_format, 1, test_res ? "PASSED" : "FAILED",
          test_res ? "The array is sorted by distance"
                   : "The array is not sorted by distance");

  test_res = test_same_objects_bubble_sort (start, end);
  printf (test_result_format, 2, test_res ? "PASSED" : "FAILED",
          test_res ? "The array has the same items after sorting"
                   : "The array does not have the same items after sorting");

  test_res = test_quick_sort (start, end);
  printf (test_result_format, 3, test_res ? "PASSED" : "FAILED",
          test_res ? "The array is sorted by duration"
                   : "The array is not sorted by duration");

  test_res = test_same_objects_quick_sort (start, end);
  printf (test_result_format, 4, test_res ? "PASSED" : "FAILED",
          test_res ? "The array has the same items after sorting"
                   : "The array does not have the same items after sorting");
}