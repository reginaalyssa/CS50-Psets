/**
 * Encrypts a message using Vigenere's cipher.
 *
 * Usage: vigenere k
 * Input p
 *
 * where k is a string comprised only of alphabetical characters
 * where each character in k corresponds to a number (A and a as 0,
 * B and b as 1, ..., and Z and z as 25)
 * and
 * where p is the string or message to be encrypted.
 *
 * Each of the alphabetical characters in message p, denoted as
 * p[i] (i from 0 to length of p), will be shifted or rotated
 * k[j] times (j from 0 to length of k). If length of k is less
 * than the length of p, j will wrap around back to 0 until all
 * characters in p have been encryped.
 */

#include <stdio.h>
#include <cs50.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

char rotate(char c, char k);

int main(int argc, string argv[])
{
    // ensure proper number of arguments
    if (argc != 2)
    {
        printf("Usage: ./vigenere k\n");
        return 1;
    }

    // ensure all characters in key are alphabetical
    for (int i = 0, n = strlen(argv[1]); i < n; i++)
    {
        if (!isalpha(argv[1][i]))
        {
            printf("Usage: ./vigenere k\n");
            return 1;
        }
    }
    string k = argv[1];

    // get message to be encrypted
    printf("plaintext: ");
    string p = get_string();

    printf("ciphertext: ");
    int shift_count = 0;
    for (int i = 0, n = strlen(p); i < n; i++)
    {
        // rotate alphabetical characters by k[j]'s equivalent number value positions
        if (isalpha(p[i]))
        {
            int j = shift_count % strlen(k);
            p[i] = rotate(p[i], k[j]);
            shift_count++;
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
char rotate(char c, char k)
{
    // determine first letter of alphabet for message
    char first_letter_msg = '\0';
    if (islower(c))
    {
        first_letter_msg = 'a';
    }
    else if (isupper(c))
    {
        first_letter_msg = 'A';
    }

    // determine first letter of alphabet for key
    char first_letter_key = '\0';
    if (islower(k))
    {
        first_letter_key = 'a';
    }
    else if (isupper(k))
    {
        first_letter_key = 'A';
    }

    // calculate new character after shift
    return ((c - first_letter_msg + k - first_letter_key) % 26) + first_letter_msg;
}