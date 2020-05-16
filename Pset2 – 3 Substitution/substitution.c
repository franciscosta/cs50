#include <stdio.h>
#include <cs50.h>
#import <string.h>
#import <ctype.h>
#import <stdlib.h>

int main(int argc, string argv[])
{

    // --------------------- KEY VALIDATION -----------------------

    // #0: Check that there is an argument

    if (argc < 2 || argc > 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    int length = strlen(argv[1]);



    // #1: Check the key length is 26 characters long

    if (length < 26 || length > 26)
    {
        printf("The key must contain 26 characters.\n");
        return 1;
    }



    // #2: Check that all charcters are alphabetical

    for (int a = 0; a < length; a++)
    {

        if (isalpha(argv[1][a]) < 1)
        {
            printf("Key must only contain alphabetic characters.\n");
            return 1;
        }

    }

    // #3: Check that all letters are unique

    int counter = 0;

    for (int y = 0; y < length; y++)
    {
        for (int z = 0; z < length; z++)
        {
            if (argv[1][y] == argv[1][z])
            {
                counter++;
            }
        }
    }

    // If they repeat more than once the number will be higher than 26

    if (counter > 26)
    {
        printf("Key must not contain repeated characters.\n");
        return 1;
    }



    // --------------------- KEY IS GOOD TO GO -----------------------


    // Get the plaintext string

    string plaintext = get_string("plaintext: ");

    string cyphertext = plaintext;

    int ptlength = strlen(plaintext); // working till here


    // Matching

    int character = 0;

    char cyphercharacter;

    for (int d = 0; d < ptlength; d++)
    {

        if (islower(plaintext[d]) > 0)
        {

            character = plaintext[d] - 97;

            cyphercharacter = tolower(argv[1][character]);

            cyphertext[d] = cyphercharacter;

            character = 0;

        }

        else if (isupper(plaintext[d]) > 0)
        {

            character = plaintext[d] - 65;

            cyphercharacter = toupper(argv[1][character]);

            cyphertext[d] = cyphercharacter;

            character = 0;

        }

    }

    // Print cyphertext

    printf("ciphertext: %s\n", cyphertext);

    printf("\n");

    // Over and out!

    return 0;

}