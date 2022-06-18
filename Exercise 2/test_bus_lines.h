#ifndef EX2_REPO_TESTBUSLINES_H
#define EX2_REPO_TESTBUSLINES_H

#include "sort_bus_lines.h"

/**
 * Tests the bubble sort algorithm - creates a copy of the given array,
 * with dynamic memory using malloc.
 *
 * It then sorts the array with bubble sort, checks if the array is sorted
 * by distance and releases the memory.
 *
 * @param start the start of the array
 * @param end the end of the array
 * @return true if the test is successful, false otherwise
 */
bool test_bubble_sort (BusLine *start, BusLine *end);

/**
 * Tests the bubble sort algorithm - creates a copy of the given array,
 * with dynamic memory using malloc.
 *
 * It then sorts the array with bubble sort, checks if the all elements of
 * the original array are in the sorted one and releases the memory.
 *
 * @param start the start of the array
 * @param end the end of the array
 * @return true if the test is successful, false otherwise
 */
bool test_same_objects_bubble_sort (BusLine *start, BusLine *end);

/**
 * Tests the quick sort algorithm - creates a copy of the given array,
 * with dynamic memory using malloc.
 *
 * It then sorts the array with quick sort, checks if the array is sorted
 * by distance and releases the memory.
 *
 * @param start the start of the array
 * @param end the end of the array
 * @return true if the test is successful, false otherwise
 */
bool test_quick_sort (BusLine *start, BusLine *end);

/**
 * Tests the quick sort algorithm - creates a copy of the given array,
 * with dynamic memory using malloc.
 *
 * It then sorts the array with quick sort, checks if the all elements of
 * the original array are in the sorted one and releases the memory.
 *
 * @param start the start of the array
 * @param end the end of the array
 * @return true if the test is successful, false otherwise
 */
bool test_same_objects_quick_sort (BusLine *start, BusLine *end);

/**
 * Checks if all bus lines between start and end are sorted by distance.
 *
 * @param start the start of the array
 * @param end the end of the array
 * @return 1 if the array is sorted by distance, 0 otherwise
 */
int is_sorted_by_distance (BusLine *start, BusLine *end);

/**
 * Checks if all bus lines between start and end are sorted by duration.
 *
 * @param start the start of the array
 * @param end the end of the array
 * @return 1 if the array is sorted by duration, 0 otherwise
 */
int is_sorted_by_duration (BusLine *start, BusLine *end);

/**
 * Checks if the given 2 arrays contain the same elements (might have differnet
 * order though).
 *
 * @param start_sorted the start of the first array
 * @param end_sorted the end of the first array
 * @param start_original the start of the second array
 * @param end_original the end of the second array
 * @return 1 if the arrays are equal, 0 otherwise
 */
int is_equal (BusLine *start_sorted, BusLine *end_sorted,
              BusLine *start_original, BusLine *end_original);

/**
 * Checks if the given array (through start) contain element target.
 *
 * @param start the start of the array
 * @param length the amount of elements in the array
 * @param target the target element to find
 * @return 1 if the array contains target element, 0 otherwise
 */
int contains (BusLine *start, int length, BusLine target);

/**
 * Copies the given array to a new array, with dynamic memory using malloc.
 * It DOES NOT free the allocated memory but rather passes it to the value
 * of start_copy_ptr
 *
 * @param start the start of the array to copy elements from
 * @param end the end of the array to copy elements from
 * @param start_copy_ptr a pointer to a pointer to the start of the new array
 *                       that's going to be allocated
 * @param end_copy_ptr a pointer to a pointer to the end of the new array
 * @return 1 if the copy was successful, 0 otherwise
 */
int copy_array (BusLine *start, BusLine *end, BusLine **start_copy_ptr,
                BusLine **end_copy_tr);

#endif // EX2_REPO_TESTBUSLINES_H
