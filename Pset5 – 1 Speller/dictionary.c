// Implements a dictionary's functionality

#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 5000;
const unsigned int R = 31;


// Hash table
node *table[N];


// Word count
int wordcount = 0;


// Returns true if word is in dictionary else false
bool check(const char *word)
{

    // Converting the word into lowercase myself
    // So I can hash it successfully!

    // Get lentgh of the word
    int length = strlen(word);

    // Create a lowercase copy of the word
    char lowercase[length + 1];

    // Convert all chars to lowercase
    for (int a = 0; a < length + 1; a++)
    {
        lowercase[a] = tolower(word[a]);
    }

    // Open a pointer to that location of the table
    node *check = table[hash(lowercase)];

    // If the pointer is NULL abort
    if (check == NULL)
    {
        return false;
    }

    // If its not NULL
    while (check != NULL)
    {

        if (strcasecmp(check->word, lowercase) == 0)
        {
            // Success
            return true;
        }

        // If not found, continue onto the next one
        check = check->next;

    }

    // Free the pointer
    free(check);

    // If not found
    return false;

}


// Hashes word to a number

unsigned int hash(const char *word)
{

    // This is where I got my hash formula from:
    // https://algs4.cs.princeton.edu/34hash/

    // Set up my variables
    unsigned int hash = 0, word_length = strlen(word);

    // Cycle through the word and apply my formula
    for (int a = 0; a < word_length; a++)
    {
        hash = (R * hash + word[a]) % N;
    }

    return hash;

}


// Loads dictionary into memory, returning true if successful else false

bool load(const char *dictionary)
{

    // Initialize the elemebts in the table
    for (int a = 0; a < N; a++)
    {
       table[a] = NULL;
    }

    // Open the dictionary
    FILE *dic = fopen(dictionary, "r");

    // Make sure its not NULL
    if (dic == NULL)
    {
        return false;
    }

    // Set up a container to store the words as I load them
    char word[LENGTH];

    unsigned int hashvalue = 0, counter = 0;

    // While
    for (char c = fgetc(dic); c != EOF; c = fgetc(dic))
    {

        // If it's not an end of word
       if (c != '\n')
       {
            word[counter] = c; // store the word
            counter++; // Increase the position
       }

       else if (c == '\n')
       {

            // Terminate the word
            word[counter] = '\0';

            // Reset counter
            counter = 0;

            // Setup my node to store the word
            node *n = malloc(sizeof(node));

            // Copy my word into the node
            strcpy(n->word, word);
            n->next = NULL;

            // Hash the word
            hashvalue = hash(word);

            // If the slot is taken
            if (table[hashvalue] != NULL)
            {
                // Point to the current word
                n->next = table[hashvalue];
            }

            // And then have the head point to the new word
            table[hashvalue] = n;

            // Increase word count
            wordcount++;

       }

    }

    fclose(dic);

    return true;

}


// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return wordcount;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{

    for (int a = 0; a < N; a++)
    {
        // While my bucket is full
        while (table[a] != NULL)
        {
            // Point where it points to
            node *n = table[a];

            // Update it to traverse forward to the next node
            table[a] = table[a]->next;

            // Free the previous node
            free(n);
        }
    }


    return true;

}
