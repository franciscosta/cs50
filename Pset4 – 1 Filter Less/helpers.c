#include <math.h>
#include "helpers.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{

    float red, green, blue;
    float average = 0;

    for (int a = 0; a < height; a++)
    {

        for (int b = 0; b < width; b++)
        {

            // Getting the three values
            red = image[a][b].rgbtRed;
            green = image[a][b].rgbtGreen;
            blue = image[a][b].rgbtBlue;

            // Rounding them up
            average = round((red + green + blue) / 3);

            // Re-assigning the new value to all three channels
            image[a][b].rgbtBlue = (int) average;
            image[a][b].rgbtGreen = (int) average;
            image[a][b].rgbtRed = (int) average;

        }

    }

    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{

    // MY floats
    float sepiaRed, sepiaGreen, sepiaBlue;
    float originalRed, originalGreen, originalBlue;

    for (int a = 0; a < height; a++)
    {

        for (int b = 0; b < width; b++)
        {

            // Getting the values
            originalRed = image[a][b].rgbtRed;
            originalGreen = image[a][b].rgbtGreen;
            originalBlue = image[a][b].rgbtBlue;

            // Applying the formula to get the sepia equivalent
            sepiaRed = round((0.393 * originalRed) + (0.769 * originalGreen) + (0.189 * originalBlue));
            sepiaGreen = round((0.349 * originalRed) + (0.686 * originalGreen) + (0.168 * originalBlue));
            sepiaBlue = round((0.272 * originalRed) + (0.534 * originalGreen) + (0.131 * originalBlue));

            // If satements to solve edge cases where ...
            // ... the result from the formula is more than 255
            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }

            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }

            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }

            // If all is right, cast results as ints and assign them
            image[a][b].rgbtRed = (int) sepiaRed;
            image[a][b].rgbtGreen = (int) sepiaGreen;
            image[a][b].rgbtBlue = (int) sepiaBlue;

        }

    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{

    int counter = width - 1;

    RGBTRIPLE swapArray[width];

    for (int a = 0; a < height; a++)
    {
        for (int b = 0; b < width; b++)
        {
            // Copy the image
            swapArray[b].rgbtRed = image[a][b].rgbtRed;
            swapArray[b].rgbtGreen = image[a][b].rgbtGreen;
            swapArray[b].rgbtBlue = image[a][b].rgbtBlue;
        }

        for (int c = 0; c < width; c++)
        {

            // Assign the image in reverse
            // Hence the use of counter
            // To go from last to first number in the array
            // While the image is going from first to last
            image[a][c].rgbtRed = swapArray[counter].rgbtRed;
            image[a][c].rgbtGreen = swapArray[counter].rgbtGreen;
            image[a][c].rgbtBlue = swapArray[counter].rgbtBlue;
            counter--;
        }
        counter = width - 1;
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{

    // I'm creating a copy of the image
    // So I can do the maths based on the original
    // While storing them on the new array
    // This so new values don't impact my averages
    RGBTRIPLE swapArray[height][width];

    float red, green, blue;

    for (int a = 0; a < height; a++)
    {

        for (int b = 0; b < width; b++)
        {
            // ------------ Corner Pixels

            // Upper left corner
            if (a == 0 && b == 0)
            {

                // I'm casting the calculation of the average as a float
                // So I can get decimals from it
                // Otherwise i would loose all decimals
                // Because each .rgbtRed is an int

                red = (float)
                      (image[a][b].rgbtRed +
                       image[a][b + 1].rgbtRed +
                       image[a + 1][b].rgbtRed +
                       image[a + 1][b + 1].rgbtRed) / 4;

                green = (float)
                        (image[a][b].rgbtGreen +
                         image[a][b + 1].rgbtGreen +
                         image[a + 1][b].rgbtGreen +
                         image[a + 1][b + 1].rgbtGreen) / 4;


                blue = (float)
                       (image[a][b].rgbtBlue +
                        image[a][b + 1].rgbtBlue +
                        image[a + 1][b].rgbtBlue +
                        image[a + 1][b + 1].rgbtBlue) / 4;

                // Round at the end
                // No need to cast as int
                // Round already eliminates all decimals
                swapArray[a][b].rgbtRed = round(red);
                swapArray[a][b].rgbtGreen = round(green);
                swapArray[a][b].rgbtBlue = round(blue);

            }

            // Upper right corner
            else if (a == 0 && b == width - 1)
            {

                red = (float)
                      (image[a][b].rgbtRed +
                       image[a][b - 1].rgbtRed +
                       image[a + 1][b].rgbtRed +
                       image[a + 1][b - 1].rgbtRed) / 4;

                green = (float)
                        (image[a][b].rgbtGreen +
                         image[a][b - 1].rgbtGreen +
                         image[a + 1][b].rgbtGreen +
                         image[a + 1][b - 1].rgbtGreen) / 4;

                blue = (float)
                       (image[a][b].rgbtBlue +
                        image[a][b - 1].rgbtBlue +
                        image[a + 1][b].rgbtBlue +
                        image[a + 1][b - 1].rgbtBlue) / 4;

                swapArray[a][b].rgbtRed = round(red);
                swapArray[a][b].rgbtGreen = round(green);
                swapArray[a][b].rgbtBlue = round(blue);

            }

            // Lower left corner
            else if (a == height - 1 && b == 0)
            {

                red = (float)
                      (image[a][b].rgbtRed +
                       image[a][b + 1].rgbtRed +
                       image[a - 1][b].rgbtRed +
                       image[a - 1][b + 1].rgbtRed) / 4;

                green = (float)
                        (image[a][b].rgbtGreen +
                         image[a][b + 1].rgbtGreen +
                         image[a - 1][b].rgbtGreen +
                         image[a - 1][b + 1].rgbtGreen) / 4;

                blue = (float)
                       (image[a][b].rgbtBlue +
                        image[a][b + 1].rgbtBlue +
                        image[a - 1][b].rgbtBlue +
                        image[a - 1][b + 1].rgbtBlue) / 4;

                swapArray[a][b].rgbtRed = round(red);
                swapArray[a][b].rgbtGreen = round(green);
                swapArray[a][b].rgbtBlue = round(blue);

            }

            // Lower right corner
            else if (a == height - 1 && b == width - 1)
            {

                red = (float)
                      (image[a][b].rgbtRed +
                       image[a][b - 1].rgbtRed +
                       image[a - 1][b].rgbtRed +
                       image[a - 1][b - 1].rgbtRed) / 4;

                green = (float)
                        (image[a][b].rgbtGreen +
                         image[a][b - 1].rgbtGreen +
                         image[a - 1][b].rgbtGreen +
                         image[a - 1][b - 1].rgbtGreen) / 4;

                blue = (float)
                       (image[a][b].rgbtBlue +
                        image[a][b - 1].rgbtBlue +
                        image[a - 1][b].rgbtBlue +
                        image[a - 1][b - 1].rgbtBlue) / 4;

                swapArray[a][b].rgbtRed = round(red);
                swapArray[a][b].rgbtGreen = round(green);
                swapArray[a][b].rgbtBlue = round(blue);

            }

            // ----------- Edges

            // Left edge
            else if (a == 0 && b != 0 && b != width - 1)
            {

                red = (float)
                      (image[a][b].rgbtRed +
                       image[a][b + 1].rgbtRed +
                       image[a][b - 1].rgbtRed +
                       image[a + 1][b].rgbtRed +
                       image[a + 1][b + 1].rgbtRed +
                       image[a + 1][b - 1].rgbtRed) / 6;

                green = (float)
                        (image[a][b].rgbtGreen +
                         image[a][b + 1].rgbtGreen +
                         image[a][b - 1].rgbtGreen +
                         image[a + 1][b].rgbtGreen +
                         image[a + 1][b + 1].rgbtGreen +
                         image[a + 1][b - 1].rgbtGreen) / 6;

                blue = (float)
                       (image[a][b].rgbtBlue +
                        image[a][b + 1].rgbtBlue +
                        image[a][b - 1].rgbtBlue +
                        image[a + 1][b].rgbtBlue +
                        image[a + 1][b + 1].rgbtBlue +
                        image[a + 1][b - 1].rgbtBlue) / 6;

                swapArray[a][b].rgbtRed = round(red);
                swapArray[a][b].rgbtGreen = round(green);
                swapArray[a][b].rgbtBlue = round(blue);

            }

            // Right edge
            else if (a == height - 1 && b != 0 && b != width - 1)
            {

                red = (float)
                      (image[a][b].rgbtRed +
                       image[a][b + 1].rgbtRed +
                       image[a][b - 1].rgbtRed +
                       image[a - 1][b].rgbtRed +
                       image[a - 1][b + 1].rgbtRed +
                       image[a - 1][b - 1].rgbtRed) / 6;

                green = (float)
                        (image[a][b].rgbtGreen +
                         image[a][b + 1].rgbtGreen +
                         image[a][b - 1].rgbtGreen +
                         image[a - 1][b].rgbtGreen +
                         image[a - 1][b + 1].rgbtGreen +
                         image[a - 1][b - 1].rgbtGreen) / 6;

                blue = (float)
                       (image[a][b].rgbtBlue +
                        image[a][b + 1].rgbtBlue +
                        image[a][b - 1].rgbtBlue +
                        image[a - 1][b].rgbtBlue +
                        image[a - 1][b + 1].rgbtBlue +
                        image[a - 1][b - 1].rgbtBlue) / 6;

                swapArray[a][b].rgbtRed = round(red);
                swapArray[a][b].rgbtGreen = round(green);
                swapArray[a][b].rgbtBlue = round(blue);

            }

            // Top edge
            else if (a != 0 && a != height - 1 && b == 0)
            {

                red = (float)
                      (image[a][b].rgbtRed +
                       image[a + 1][b].rgbtRed +
                       image[a - 1][b].rgbtRed +
                       image[a][b + 1].rgbtRed +
                       image[a + 1][b + 1].rgbtRed +
                       image[a - 1][b + 1].rgbtRed) / 6;

                green = (float)
                        (image[a][b].rgbtGreen +
                         image[a + 1][b].rgbtGreen +
                         image[a - 1][b].rgbtGreen +
                         image[a][b + 1].rgbtGreen +
                         image[a + 1][b + 1].rgbtGreen +
                         image[a - 1][b + 1].rgbtGreen) / 6;

                blue = (float)
                       (image[a][b].rgbtBlue +
                        image[a + 1][b].rgbtBlue +
                        image[a - 1][b].rgbtBlue +
                        image[a][b + 1].rgbtBlue +
                        image[a + 1][b + 1].rgbtBlue +
                        image[a - 1][b + 1].rgbtBlue) / 6;

                swapArray[a][b].rgbtRed = round(red);
                swapArray[a][b].rgbtGreen =  round(green);
                swapArray[a][b].rgbtBlue = round(blue);

            }

            // Bottom edge
            else if (a != 0 && a != height - 1 && b == width - 1)
            {

                red = (float)
                      (image[a][b].rgbtRed +
                       image[a + 1][b].rgbtRed +
                       image[a - 1][b].rgbtRed +
                       image[a][b - 1].rgbtRed +
                       image[a + 1][b - 1].rgbtRed +
                       image[a - 1][b - 1].rgbtRed) / 6;

                green = (float)
                        (image[a][b].rgbtGreen +
                         image[a + 1][b].rgbtGreen +
                         image[a - 1][b].rgbtGreen +
                         image[a][b - 1].rgbtGreen +
                         image[a + 1][b - 1].rgbtGreen +
                         image[a - 1][b - 1].rgbtGreen) / 6;

                blue = (float)
                       (image[a][b].rgbtBlue +
                        image[a + 1][b].rgbtBlue +
                        image[a - 1][b].rgbtBlue +
                        image[a][b - 1].rgbtBlue +
                        image[a + 1][b - 1].rgbtBlue +
                        image[a - 1][b - 1].rgbtBlue) / 6;

                swapArray[a][b].rgbtRed = round(red);
                swapArray[a][b].rgbtGreen = round(green);
                swapArray[a][b].rgbtBlue = round(blue);

            }

            // Middle Pixels
            else
            {

                red = (float)
                      (image[a][b].rgbtRed +
                       image[a][b + 1].rgbtRed +
                       image[a][b - 1].rgbtRed +
                       image[a + 1][b].rgbtRed +
                       image[a + 1][b + 1].rgbtRed +
                       image[a + 1][b - 1].rgbtRed +
                       image[a - 1][b].rgbtRed +
                       image[a - 1][b + 1].rgbtRed +
                       image[a - 1][b - 1].rgbtRed) / 9;

                green = (float)
                        (image[a][b].rgbtGreen +
                         image[a][b + 1].rgbtGreen +
                         image[a][b - 1].rgbtGreen +
                         image[a + 1][b].rgbtGreen +
                         image[a + 1][b + 1].rgbtGreen +
                         image[a + 1][b - 1].rgbtGreen +
                         image[a - 1][b].rgbtGreen +
                         image[a - 1][b + 1].rgbtGreen +
                         image[a - 1][b - 1].rgbtGreen) / 9;

                blue = (float)
                       (image[a][b].rgbtBlue +
                        image[a][b + 1].rgbtBlue +
                        image[a][b - 1].rgbtBlue +
                        image[a + 1][b].rgbtBlue +
                        image[a + 1][b + 1].rgbtBlue +
                        image[a + 1][b - 1].rgbtBlue +
                        image[a - 1][b].rgbtBlue +
                        image[a - 1][b + 1].rgbtBlue +
                        image[a - 1][b - 1].rgbtBlue) / 9;

                swapArray[a][b].rgbtRed =  round(red);
                swapArray[a][b].rgbtGreen =  round(green);
                swapArray[a][b].rgbtBlue =  round(blue);

            }

        }

    }

    // Assign all values to originak image
    for (int c = 0; c < height; c++)
    {
        for (int d = 0; d < width; d++)
        {
            image[c][d].rgbtRed = swapArray[c][d].rgbtRed;
            image[c][d].rgbtGreen = swapArray[c][d].rgbtGreen;
            image[c][d].rgbtBlue = swapArray[c][d].rgbtBlue;
        }
    }

    return;
}
