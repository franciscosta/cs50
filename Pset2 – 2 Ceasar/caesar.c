#include <stdio.h>
#include <cs50.h>
#import <string.h>
#import <ctype.h>
#import <stdlib.h>

int main(int argc, string argv[])
{


    if (argc < 2 || argc > 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }


    // Check if any of the chars in the string is non-numeric

    int length = strlen(argv[1]);

    bool check = true;

    for (int a = 0; a < length; a++)
    {

        // Is digit == 0 when the char us non-numeric
        // Therefore, I'm just checking if at any point in the string there's a non numeric in there
        // This protects me against any kind of characters

        if (isdigit(argv[1][a]) < 1)
        {
            check = false;
        }

    }

    // If non-numeric then return error message

    if (check == false)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    // If all chars are numeric

    if (check == true)
    {

        // Convert argument into integer

        int k = atoi(argv[1]);


        // Get the plain text from the user

        string plaintext = get_string("plaintext: ");

        int ptlength = strlen(plaintext);

        int index;
        int c;
        int final;


        for (int b = 0; b < ptlength; b++)
        {
            if (islower(plaintext[b]) > 0)
            {


                // Places the character on a scale of 26 characters

                index = plaintext[b] - 96;

                // Executes the formula

                c = (index + k) % 26;

                // Restores the value

                final = c + 96;

                plaintext[b] = final;


            }

            else if (isupper(plaintext[b]) > 0)
            {

                index = plaintext[b] - 64;

                c = (index + k) % 26;

                final = c + 64;

                plaintext[b] = final;

            }
        }

        printf("ciphertext: %s\n", plaintext);


    }



}