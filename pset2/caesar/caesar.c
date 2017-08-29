/**
 * Encrypts a message using Caesar's cipher.
 *
 * Usage: caesar k
 * Input p
 *
 * where k is an integer that acts as key for
 * the number of shifts/rotations per alphabetical
 * character in the message.
 * and
 * where p is the string or message to be encrypted.
 */

#include <stdio.h>
#include <cs50.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

char rotate(char c, int k);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: ./caesar k\n");
        return 1;
    }

    // get key
    int k = atoi(argv[1]);

    // get message to encrypt
    printf("plaintext: ");
    string p = get_string();

    // iterate over each character in message
    printf("ciphertext: ");
    for (int i = 0, n = strlen(p); i < n; i++)
    {
        // rotate alphabetical character by k positions
        if (isalpha(p[i]))
        {
            p[i] = rotate(p[i], k);
        }
        printf("%c", p[i]);
    }
    printf("\n");

    // success
    return 0;
}

/**
 * Rotates character c by k positions.
 */
char rotate(char c, int k)
{
    // first letter of alphabet based on case
    char first_letter = '\0';
    if (islower(c))
    {
        first_letter = 'a';
    }
    else if (isupper(c))
    {
        first_letter = 'A';
    }

    // calculate new character after shift
    return ((c - first_letter + k) % 26) + first_letter;
}