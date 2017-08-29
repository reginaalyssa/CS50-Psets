/**
 * Prints initials given a name.
 */

#include<stdio.h>
#include<string.h>
#include<cs50.h>

char uppercase(char letter);

int main(void)
{
    // get string input
    string name = get_string();

    // parse through name
    int i = 0;
    while (i < strlen(name))
    {
        // skip whitespace characters
        while (name[i] == ' ')
        {
            i++;
        }

        // print uppercase of first character after whitespace
        if (i < strlen(name))
        {
            printf("%c", uppercase(name[i]));
        }

        // skip the rest of the characters in name
        while (name[i] != ' ' && i < strlen(name))
        {
            i++;
        }
    }
    printf("\n");
}

/**
 * Prints alphabetical character in uppercase.
 */
char uppercase(char letter)
{
    // convert lowercase character to uppercase
    if (letter <= 'z' && letter >= 'a')
    {
        return letter - ('a' -  'A');
    }
    // return uppercase characters as is
    else
    {
        return letter;
    }
}