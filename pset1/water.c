/**
 * Determines water usage by calculating amount of bottles of
 * drinking water equivalent to every minute in the shower.
 */

#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // ensure input is nonnegative
    int minutes;
    do
    {
        printf("Minutes: ");
        minutes = get_int();
    }
    while (minutes < 0);

    // calculate number of bottles using minutes in shower
    printf("Bottles: %d\n", minutes * 12);

    // success
    return 0;
}