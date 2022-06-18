#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sort_bus_lines.h"
#include "test_bus_lines.h"

// [=== TESTS ===]

bool test_bubble_sort (BusLine *start, BusLine *end)
{
  BusLine *start_copy, *end_copy;
  if (!copy_array (start, end, &start_copy, &end_copy))
  {
    return false;
  }

  bubble_sort (start_copy, end_copy);

  int result = is_sorted_by_distance (start_copy, end_copy);
  free (start_copy);

  return result != 0;
}

bool test_same_objects_bubble_sort (BusLine *start, BusLine *end)
{
  BusLine *start_copy, *end_copy;
  if (!copy_array (start, end, &start_copy, &end_copy))
  {
    return false;
  }

  bubble_sort (start_copy, end_copy);

  int result = is_equal (start_copy, end_copy, start, end);
  free (start_copy);

  return result != 0;
}

bool test_quick_sort (BusLine *start, BusLine *end)
{
  BusLine *start_copy, *end_copy;
  if (!copy_array (start, end, &start_copy, &end_copy))
  {
    return false;
  }

  quick_sort (start_copy, end_copy);

  int result = is_sorted_by_duration (start_copy, end_copy);
  free (start_copy);

  return result != 0;
}

bool test_same_objects_quick_sort (BusLine *start, BusLine *end)
{
  BusLine *start_copy, *end_copy;
  if (!copy_array (start, end, &start_copy, &end_copy))
  {
    return false;
  }

  quick_sort (start_copy, end_copy);

  int result = is_equal (start_copy, end_copy, start, end);
  free (start_copy);

  return result != 0;
}

// [=== CHECKS ===]

int is_sorted_by_distance (BusLine *start, BusLine *end)
{
  int length = get_number_of_elements (start, end);

  for (int i = 0; i < length - 1; i++)
  {
    if (start[i].distance > start[i + 1].distance)
    {
      return 0;
    }
  }

  return 1;
}

int is_sorted_by_duration (BusLine *start, BusLine *end)
{
  int length = get_number_of_elements (start, end);

  for (int i = 0; i < length - 1; i++)
  {
    if (start[i].duration > start[i + 1].duration)
    {
      return 0;
    }
  }

  return 1;
}

int is_equal (BusLine *start_sorted, BusLine *end_sorted,
              BusLine *start_original, BusLine *end_original)
{
  int length_original = get_number_of_elements (start_original, end_original);
  int length_sorted = get_number_of_elements (start_sorted, end_sorted);

  if (length_original != length_sorted)
  {
    return 0;
  }

  for (int i = 0; i < length_original; i++)
  {
    if (contains (start_sorted, length_sorted, start_original[i]) == false)
    {
      return 0;
    }
  }

  return 1;
}

int contains (BusLine *start, int length, BusLine target)
{
  for (int j = 0; j < length; j++)
  {
    if (start[j].line_number == target.line_number
        && start[j].distance == target.distance
        && start[j].duration == target.duration)
    {
      return 1;
    }
  }
  return 0;
}

// [=== UTILITIES ===]

int copy_array (BusLine *start, BusLine *end, BusLine **start_copy_ptr,
                BusLine **end_copy_tr)
{
  int length = get_number_of_elements (start, end);

  *start_copy_ptr = malloc (length * sizeof (BusLine));
  if (start_copy_ptr == NULL)
  {
    return 0;
  }

  *end_copy_tr = (*start_copy_ptr) + length;

  memcpy ((*start_copy_ptr), start, length * sizeof (BusLine));
  return 1;
}