#ifndef EX2_REPO_SORTBUSLINES_H
#define EX2_REPO_SORTBUSLINES_H

/**
 * A BusLine struct containing a line number, distance from the campus and
 * duration to get to the campus.
 */
typedef struct BusLine
{
  int line_number, distance, duration;
} BusLine;

/**
 * An implementation of the Bubble-Sort algorithm.
 * This algorithm is using pointers only.
 *
 * @param start the start of the array to sort
 * @param end the end of the array to sort
 */
void bubble_sort (BusLine *start, BusLine *end);

/**
 * An implementation of the Quick-Sort algorithm.
 * This algorithm is using pointers only.
 *
 * @param start the start of the array to sort
 * @param end the end of the array to sort
 */
void quick_sort (BusLine *start, BusLine *end);

/**
 * Hanldes the partition part of quick-sort.
 * It selects a pivot (the last element) and moves all elements smaller than
 * pivot to itself, and all elements bigger than pivot to the right.
 *
 * @param start start of the array to sort
 * @param end end of the array to sort
 * @return a pointer to the pivot which is now in the correct position
 */
BusLine *partition (BusLine *start, BusLine *end);

/**
 * An algorithm to swap the values of the given two pointers.
 *
 * @param a pointer to the first element
 * @param b pointer to the second element
 */
void swap (BusLine *a, BusLine *b);

/**
 * Returns the number of elements between start and end
 *
 * @param start the start of the array
 * @param end the end of the array
 */
int get_number_of_elements (BusLine *start, BusLine *end);

#endif // EX2_REPO_SORTBUSLINES_H
