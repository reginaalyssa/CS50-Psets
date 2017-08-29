/**
 * Cracks passwords using C's DES-based crypt function.
 *
 * Usage: crack hash
 *
 * where hash can be generated given the right password and salt
 * using crypt(password, salt).
 *
 * The salt used is "50" in this case while the password is generated
 * from all possible n-lettered words (n from 1 to 4). Once the value
 * generated from crypt(password, salt) is equal to the hash input,
 * the password is found and printed.
 */

#define _XOPEN_SOURCE

#include <unistd.h>
#include <stdio.h>
#include <cs50.h>
#include <string.h>

void generatePassword(int i, int n, string password, string salt, string hash);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: ./crack hash\n");
        return 1;
    }

    // initialize salt and password values
    string salt = "50";
    string password = "";

    // generate all possible 1 to 4 letter passwords
    generatePassword(0, 4, password, salt, argv[1]);

    return 0;
}

/**
 * Generates all possible 1 to n letter passwords using recursion.
 *
 * where i is the current length of the string password and
 * n is the maximum password length (4 in this case).
 */
void generatePassword(int i, int n, string password, string salt, string hash)
{
    if (i <= n)
    {
        // exit when password is found
        if (strcmp(crypt(password, salt), hash) == 0)
        {
            printf("%s\n", password);
            exit(0);
        }
        else
        {
            char temp[50];
            char append[2];

            // append alphabetical characters 'a' to 'z' to current password
            for (int c = 'a'; c <= 'z'; c++)
            {
                strcpy(temp, password);
                append[0] = (char) c;
                append[1] = 0;
                strcat(temp, append);

                // call function with newly appended password
                generatePassword(i + 1, n, temp, salt, hash);
            }

            // append alphabetical characters 'A' to 'Z' to current password
            for (int c = 'A'; c <= 'Z'; c++)
            {
                strcpy(temp, password);
                append[0] = (char) c;
                append[1] = 0;
                strcat(temp, append);

                // call function with newly appended password
                generatePassword(i + 1, n, temp, salt, hash);
            }
        }
    }
}