#include "helpers.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
RGBTRIPLE CalculateAverageForPixel(RGBTRIPLE pixel);
RGBTRIPLE SetSepiaValuesForPixel(RGBTRIPLE pixel);
RGBTRIPLE SetBlurredValuesForPixel(int height, int width, int maxHeight, int maxWidth, RGBTRIPLE image[maxHeight][maxWidth]);
bool PixelNeighbourExisting(int verticalShift, int horizontalShift, int height, int width, int maxHeight, int maxWidth);
RGBTRIPLE GetPixelNeighbour(int verticalShift, int horizontalShift, int height, int width, int maxHeight, int maxWidth, RGBTRIPLE image[maxHeight][maxWidth]);
// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE grayscaled = CalculateAverageForPixel(image[i][j]);
            image[i][j] = grayscaled;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE vintaged = SetSepiaValuesForPixel(image[i][j]);
            image[i][j] = vintaged;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE reflectedImage[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            reflectedImage[i][j] = image[i][width-1-j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = reflectedImage[i][j];
        }
    }

}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE blurredImage[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE blurred = SetBlurredValuesForPixel(i, j, height, width, image);
            blurredImage[i][j] = blurred;
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = blurredImage[i][j];
        }
    }
}


RGBTRIPLE CalculateAverageForPixel(RGBTRIPLE pixel)
{
    uint8_t average = round((pixel.rgbtBlue + pixel.rgbtGreen + pixel.rgbtRed) / 3.0);

    pixel.rgbtBlue = pixel.rgbtGreen = pixel.rgbtRed = average;
    return pixel;
}

RGBTRIPLE SetSepiaValuesForPixel(RGBTRIPLE pixel)
{
    uint16_t sepiaRed = round((0.393 * pixel.rgbtRed) + (0.769 * pixel.rgbtGreen) + (0.189 * pixel.rgbtBlue));
    uint16_t sepiaGreen = round((0.349 * pixel.rgbtRed) + (0.686 * pixel.rgbtGreen) + (0.168 * pixel.rgbtBlue));
    uint16_t sepiaBlue =  round((0.272 * pixel.rgbtRed) + (0.534 * pixel.rgbtGreen) + (0.131 * pixel.rgbtBlue));

    if (sepiaRed > 255) {
        sepiaRed = 255;
    }

    if (sepiaGreen > 255) {
        sepiaGreen = 255;
    }

    if (sepiaBlue > 255){
        sepiaBlue = 255;
    }

    pixel.rgbtRed = (uint8_t)sepiaRed;
    pixel.rgbtGreen = (uint8_t)sepiaGreen;
    pixel.rgbtBlue = (uint8_t)sepiaBlue;
    return pixel;
}

RGBTRIPLE SetBlurredValuesForPixel(int height, int width, int maxHeight, int maxWidth, RGBTRIPLE image[][maxWidth])
{
    int neighbourHeight;
    int neighbourWidth;
    int countOfValidPixels = 0;
    int countOfRed = 0;
    int countOfGreen = 0;
    int countOfBlue = 0;

    RGBTRIPLE current;
    current = GetPixelNeighbour(0, 0, height, width, maxHeight, maxWidth, image);
    countOfValidPixels = countOfValidPixels + 1;
    countOfRed = countOfRed + current.rgbtRed;
    countOfBlue = countOfBlue + current.rgbtBlue;
    countOfGreen = countOfGreen + current.rgbtGreen;


    if (PixelNeighbourExisting(0, -1, height, width, maxHeight, maxWidth))
    {
        RGBTRIPLE left;
        left = GetPixelNeighbour(0, -1, height, width, maxHeight, maxWidth, image);
        countOfValidPixels = countOfValidPixels +1;
        countOfRed = countOfRed + left.rgbtRed;
        countOfBlue = countOfBlue + left.rgbtBlue;
        countOfGreen = countOfGreen + left.rgbtGreen;
    }


    if (PixelNeighbourExisting(-1, -1, height, width, maxHeight, maxWidth))
    {
        RGBTRIPLE upperLeft;
        upperLeft = GetPixelNeighbour(-1, -1, height, width, maxHeight, maxWidth, image);
        countOfValidPixels = countOfValidPixels +1;
        countOfRed = countOfRed + upperLeft.rgbtRed;
        countOfBlue = countOfBlue + upperLeft.rgbtBlue;
        countOfGreen = countOfGreen + upperLeft.rgbtGreen;
    }


    if (PixelNeighbourExisting(-1, 0, height, width, maxHeight, maxWidth))
    {
        RGBTRIPLE upper;
        upper = GetPixelNeighbour(-1, 0, height, width, maxHeight, maxWidth, image);
        countOfValidPixels = countOfValidPixels +1;
        countOfRed = countOfRed + upper.rgbtRed;
        countOfBlue = countOfBlue + upper.rgbtBlue;
        countOfGreen = countOfGreen + upper.rgbtGreen;
    }


    if (PixelNeighbourExisting(-1, 1, height, width, maxHeight, maxWidth))
    {
        RGBTRIPLE upperRight;
        upperRight = GetPixelNeighbour(-1, 1, height, width, maxHeight, maxWidth, image);
        countOfValidPixels = countOfValidPixels +1;
        countOfRed = countOfRed + upperRight.rgbtRed;
        countOfBlue = countOfBlue + upperRight.rgbtBlue;
        countOfGreen = countOfGreen + upperRight.rgbtGreen;
    }


    if (PixelNeighbourExisting(0, 1, height, width, maxHeight, maxWidth))
    {
        RGBTRIPLE right;
        right = GetPixelNeighbour(0, 1, height, width, maxHeight, maxWidth, image);
        countOfValidPixels = countOfValidPixels +1;
        countOfRed = countOfRed + right.rgbtRed;
        countOfBlue = countOfBlue + right.rgbtBlue;
        countOfGreen = countOfGreen + right.rgbtGreen;
    }


    if (PixelNeighbourExisting(1, 1, height, width, maxHeight, maxWidth))
    {
        RGBTRIPLE lowerRight;
        lowerRight = GetPixelNeighbour(1, 1, height, width, maxHeight, maxWidth, image);
        countOfValidPixels = countOfValidPixels +1;
        countOfRed = countOfRed + lowerRight.rgbtRed;
        countOfBlue = countOfBlue + lowerRight.rgbtBlue;
        countOfGreen = countOfGreen + lowerRight.rgbtGreen;
    }

    if (PixelNeighbourExisting(1, 0, height, width, maxHeight, maxWidth))
    {
        RGBTRIPLE lower;
        lower = GetPixelNeighbour(1, 0, height, width, maxHeight, maxWidth, image);
        countOfValidPixels = countOfValidPixels +1;
        countOfRed = countOfRed + lower.rgbtRed;
        countOfBlue = countOfBlue + lower.rgbtBlue;
        countOfGreen = countOfGreen + lower.rgbtGreen;
    }

    if (PixelNeighbourExisting(1, -1, height, width, maxHeight, maxWidth))
    {
        RGBTRIPLE lowerLeft;
        lowerLeft = GetPixelNeighbour(1, -1, height, width, maxHeight, maxWidth, image);
        countOfValidPixels = countOfValidPixels +1;
        countOfRed = countOfRed + lowerLeft.rgbtRed;
        countOfBlue = countOfBlue + lowerLeft.rgbtBlue;
        countOfGreen = countOfGreen + lowerLeft.rgbtGreen;
    }

    RGBTRIPLE outputPixel;
    outputPixel.rgbtRed = (int) round(countOfRed / (double) countOfValidPixels);
    outputPixel.rgbtBlue = (int) round(countOfBlue / (double) countOfValidPixels);
    outputPixel.rgbtGreen = (int) round(countOfGreen / (double) countOfValidPixels);

    return outputPixel;
}

bool PixelNeighbourExisting(int verticalShift, int horizontalShift, int height, int width, int maxHeight, int maxWidth)
{
    int neighbourHeight = height + verticalShift;
    int neighbourWidth = width + horizontalShift;
    if (neighbourHeight < maxHeight - 1 && neighbourWidth < maxWidth - 1 && neighbourHeight >= 0 && neighbourWidth >= 0)
    {
        return true;
    }
    return false;
}

RGBTRIPLE GetPixelNeighbour(int verticalShift, int horizontalShift, int height, int width, int maxHeight, int maxWidth, RGBTRIPLE image[maxHeight][maxWidth])
{
    int neighbourHeight = height + verticalShift;
    int neighbourWidth = width + horizontalShift;
    RGBTRIPLE pixel;

    return image[neighbourHeight][neighbourWidth];
}