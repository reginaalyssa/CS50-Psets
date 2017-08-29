/**
 * Implements a dictionary's functionality.
 */

#include <stdio.h> // remove later
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "dictionary.h"

typedef struct node
{
    struct node *children[27];
    bool is_word;
}
node;

node *root;
int word_count = 0;

/**
 * Initializes all children of struct node to NULL.
 */
void init(node *ptr)
{
    for (int i = 0; i < 27; i++)
    {
        ptr->children[i] = NULL;
    }
    ptr->is_word = false;
}

/**
 * Returns index or proper position of character in
 * array children of struct node.
 *
 * (a & A = 0, b & B = 1, ..., z & Z = 25, ' = 26)
 */
int get_index(char c)
{
    // index for lowercase characters
    if (c >= 'a' && c <= 'z')
    {
        return c - 'a';
    }
    // index for uppercase characters
    else if (c >= 'A' && c <= 'Z')
    {
        return c - 'A';
    }
    // index for apostrophe character
    else
    {
        return 26;
    }
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    // start with pointer to the root
    node *curr;
    curr = root;

    // traverse the trie for each letter in word
    for (int i = 0, n = strlen(word); i <= n; i++)
    {
        // return true once last letter reached and word is in dictionary
        if (i == n && curr->is_word == true)
        {
            return true;
        }

        // get index of letter (0 for a, A....25 for z, Z; 26 for ' apostrophe)
        int index = get_index(word[i]);

        // current pointer does not have child with the current letter
        if (curr->children[index] == NULL)
        {
            // word does not exist in dictionary
            return false;
        }

        // letter was found in children of curr pointer
        // point to the child containing the letter
        curr = curr->children[index];
    }
    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // allocate memory for the trie
    root = malloc(sizeof(node));

    // initialize all children of root to be NULL
    init(root);

    // try to open dictionary
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        printf("Could not open %s.\n", dictionary);
        unload();
        return false;
    }

    // read the contents of dictionary file
    char temp[46];
    node *curr;
    while (fgets(temp, 46, dict) != NULL)
    {
        // remove trailing newline
        if (temp[strlen(temp) - 1] == '\n')
        {
            temp[strlen(temp) - 1] = '\0';
        }

        curr = root;

        // append each letter in word to trie
        for (int i = 0, n = strlen(temp); i <= n; i++)
        {
            // fully added word to trie/dictionary
            if (i == n && n != 0)
            {
                // classify word as part of dictionary
                curr->is_word = true;

                // increment count of number of words in dictionary
                word_count++;
            }

            // get index equivalent of current letter
            int index = get_index(temp[i]);

            // if next node in trie for current letter is empty
            if (curr->children[index] == NULL)
            {
                // create new node and set all its children to NULL
                curr->children[index] = malloc(sizeof(node));
                init(curr->children[index]);
            }

            // next node in trie exists, point to next node
            curr = curr->children[index];
        }
    }

    // close dictionary file
    fclose(dict);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    if (word_count)
    {
        return word_count;
    }
    else
    {
        return 0;
    }
}

/**
 * Free given node and all its child nodes.
 */
void free_children(node *curr)
{
    for (int i = 0; i < 27; i++)
    {
        if (curr->children[i] != NULL)
        {
            free_children(curr->children[i]);
        }
    }
    free(curr);
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    node *curr;
    curr = root;
    free_children(curr);

    return true;
}

