#include <stdio.h>

#include "sort_bus_lines.h"

void bubble_sort (BusLine *start, BusLine *end)
{
  int length = get_number_of_elements (start, end);

  for (int i = 0; i < length; i++)
  {
    for (int j = 0; j < (length - i - 1); j++)
    {
      if ((*(start + j)).distance > (*(start + j + 1)).distance)
      {
        swap (start + j, start + j + 1);
      }
    }
  }
}

void quick_sort (BusLine *start, BusLine *end)
{
  if (start < end)
  {
    BusLine *mid = partition (start, end);
    quick_sort (start, mid);
    quick_sort (mid + 1, end);
  }
}

BusLine *partition (BusLine *start, BusLine *end)
{
  int length = get_number_of_elements (start, end);

  int pivot = length - 1; // Setting the pivot to be the last element.
  int i = -1; // Setting the ending point of all elements smaller than pivot

  /*
    Looping over all elements.
    If the current element's duration is smaller than pivot's, we want to
    increase i (representing the amount of elements smaller than pivot)
    and swap the current element (j) with the element at index i.
    we know the current element (j) is smaller than pivot, and element i is
    larger than pivot because we've just increased it.
  */
  for (int j = 0; j < length - 1; j++)
  {
    if ((*(start + j)).duration <= (*(start + pivot)).duration)
    {
      i++;
      swap (start + i, start + j);
    }
  }

  swap (start + i + 1, start + pivot);

  return start + i + 1;
}

void swap (BusLine *a, BusLine *b)
{
  BusLine temp = *a;
  *a = *b;
  *b = temp;
}

int get_number_of_elements (BusLine *start, BusLine *end)
{
  return end - start;
}