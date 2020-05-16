#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }


    // Open the card
    FILE *card = fopen(argv[1], "r");

    // Create my bytesize buffer with 512 units on it
    BYTE buffer[512];

    // Initialize my imagecount
    int imagecount = 0;

    // Initialize my size_t for the fread()
    size_t read = 0;

    // the char for the file name
    char name[8];

    // my new image
    FILE *img = NULL;


    while (0 == 0)
    {

        // Part 0 –read the card and copy it into the buffer

        read = fread(buffer, sizeof(BYTE), 512, card);

        if (feof(card) || read == 0)
        {
            break;
        }

        // Part 3 – if a new image is found, first close the old one
        // at the start img is NULL so therefore this doesn't run

        if ((buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0) && img != NULL)
        {
            // Then I can close the previous image

            fclose(img);

            // Increase the image count for the next file

            imagecount++;

        }

        // Part 1 – if the first four bytes equate to a JPEG

        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {

            // Create the file name

            sprintf(name, "%03i.jpg", imagecount);

            // Open the new image file

            img = fopen(name, "w");

        }

        // Part 2 – write the data into the currently opened image until a new jpeg is found

        if (img != NULL)
        {
            fwrite(buffer, sizeof(BYTE), 512, img);
        }

    }


}
