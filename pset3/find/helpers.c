/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */

#include <cs50.h>
#include "helpers.h"

#define MAX_NUM 65536

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // ensure nonnegative array size
    if (n < 0)
    {
        return false;
    }

    // initialize left and right "pointers"
    int left = 0;
    int right = n - 1;
    int middle;

    // narrow down where to search in array
    while (left <= right)
    {
        // check if middle value contains element we're looking for
        middle = ((right - left) / 2) + left;
        if (value == values[middle])
        {
            return true;
        }
        // check left half of array if value is smaller than middle value
        else if (value < values[middle])
        {
            right = middle - 1;
        }
        // check right half of array if value is larger than middle value
        else if (value > values[middle])
        {
            left = middle + 1;
        }
    }
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // initialize count array
    int *count = malloc(sizeof(int) * MAX_NUM);

    // store in count number of instances of values[i] in array
    for (int i = 0; i < n; i++)
    {
        count[values[i]]++;
    }

    // populate array values[] based on array count
    int curr_index = 0;
    for (int i = 0; i < MAX_NUM; i++)
    {
        for (int j = 0; j < count[i]; j++)
        {
            values[curr_index] = i;
            curr_index++;
        }
    }

    // free memory
    free(count);
    return;
}
