/**
 * Generates a double half-pyramid of certain height.
 */

#include <stdio.h>
#include <cs50.h>

void printChar(int n, char c);

int main(void)
{
    // ensure valid integer
    int height;
    do
    {
        printf("Height: ");
        height = get_int();
    }
    while (height < 0 || height > 23);

    // if zero height print nothing
    if (height != 0)
    {
        // generate the half pyramid row by row
        int hashes = 1;
        int spaces = height - 1;
        for (int i = 0; i < height; i++)
        {
            printChar(spaces, ' ');
            printChar(hashes, '#');
            printChar(2, ' ');
            printChar(hashes, '#');
            printf("\n");
            spaces--;
            hashes++;
        }
    }

    // success
    return 0;
}

/**
 * Prints a certain character n times.
 */
void printChar(int n, char c)
{
    for (int j = 0; j < n; j++)
    {
        printf("%c", c);
    }
}