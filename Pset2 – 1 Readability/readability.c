#import <stdio.h>
#import <cs50.h>
#import <string.h>
#import <math.h>

int main(void)
{

    // Request text

    string text = get_string("Text: ");

    // Get total length of string

    int length = strlen(text);

    // Initialize my 3 core variables

    int letters = 0;
    int words = 1;
    int sentences = 0;

    for (int a = 0; a < length; a++)
    {

        // Count letters

        if (((text[a] >= 48 && text[a] <= 48) || (text[a] >= 65 && text[a] <= 90) || (text[a] >= 97 && text[a] <= 122)))
        {
            letters++;
        }

        // Count words

        else if (text[a] == 32)
        {
            words++;
        }

        // Count sentences

        else if ((text[a] == 46) || (text[a] == 33) || ((text[a] == 63) && (text[a] != 32)))
        {
            sentences++;
        }

    }

    // My averages

    float L = ((float) letters / (float) words) * 100;
    float S = ((float) sentences / (float) words) * 100;

    // Formula

    int index = round(0.0588 * L - 0.296 * S - 15.8);

    // Grades

    if (index < 1)
    {
        printf("Before Grade 1\n");
    }

    else if (index >= 16)
    {
        printf("Grade 16+\n");
    }

    else if (index >= 1 && index < 16)
    {
        printf("Grade %i\n", index);
    }

}