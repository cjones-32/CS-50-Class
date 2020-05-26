#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int avg = 0;
    //average all 3, then set all three to that value
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column ++)
        {
            avg = (int) round(((float) image[row][column].rgbtBlue + image[row][column].rgbtGreen + image[row][column].rgbtRed) / 3.0);
            image[row][column].rgbtRed = avg;
            image[row][column].rgbtGreen = avg;
            image[row][column].rgbtBlue = avg;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    int sepiaRed = 0;
    int sepiaGreen = 0;
    int sepiaBlue = 0;

    //convert that pixel by that filer, then if over 255, reduce it to 255
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column ++)
        {
            sepiaRed = round((float) .393 * image[row][column].rgbtRed + .769 * image[row][column].rgbtGreen + .189 * image[row][column].rgbtBlue);
            if (sepiaRed > 255)
                sepiaRed = 255;
            sepiaGreen = round((float) .349 * image[row][column].rgbtRed + .686 * image[row][column].rgbtGreen + .168 * image[row][column].rgbtBlue);
            if (sepiaGreen > 255)
                sepiaGreen = 255;
            sepiaBlue = round((float) .272 * image[row][column].rgbtRed + .534 * image[row][column].rgbtGreen + .131 * image[row][column].rgbtBlue);
            if (sepiaBlue > 255)
                sepiaBlue = 255;
            
            image[row][column].rgbtRed = sepiaRed;
            image[row][column].rgbtGreen = sepiaGreen;
            image[row][column].rgbtBlue = sepiaBlue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE filter[height][width];
    //store in temp array
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column ++)
        {
            filter[row][column].rgbtRed   = image[row][width - column - 1].rgbtRed;
            filter[row][column].rgbtGreen = image[row][width - column - 1].rgbtGreen;
            filter[row][column].rgbtBlue  = image[row][width - column - 1].rgbtBlue;
        }
    }
    //write temp aray back over original
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column ++)
        {
            image[row][column].rgbtRed   = filter[row][column].rgbtRed;
            image[row][column].rgbtGreen = filter[row][column].rgbtGreen;
            image[row][column].rgbtBlue  = filter[row][column].rgbtBlue;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    float tempRed = 0.0;
    float tempGreen = 0.0;
    float tempBlue = 0.0;
    RGBTRIPLE filter[height][width];
    
    //initialize filter to 0
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column ++)
        {
            filter[row][column].rgbtRed = 0;
            filter[row][column].rgbtGreen = 0;
            filter[row][column].rgbtBlue = 0;
        }
    }

    //top row, first column 
    //do it manually
    filter[0][0].rgbtRed   = round((float)(image[0][0].rgbtRed + image[0][1].rgbtRed + image[1][0].rgbtRed + image[1][1].rgbtRed) / 4);
    filter[0][0].rgbtGreen = round((float)(image[0][0].rgbtGreen + image[0][1].rgbtGreen + image[1][0].rgbtGreen + image[1][1].rgbtGreen) / 4);
    filter[0][0].rgbtBlue  = round((float)(image[0][0].rgbtBlue + image[0][1].rgbtBlue + image[1][0].rgbtBlue + image[1][1].rgbtBlue) / 4);

    //top row, second column to second to last column
    //scan threw the pixel to left, right and bottom
    for (int column = 1, w = width - 1; column < w; column++)
    {
        for (int r = 0; r < 2; r++)
        {
            for (int c = -1; c < 2; c++)
            {
                tempRed   += (float) image[0 + r][column + c].rgbtRed / 6;
                tempGreen += (float) image[0 + r][column + c].rgbtGreen / 6;
                tempBlue  += (float) image[0 + r][column + c].rgbtBlue / 6;
            }
        }
        filter[0][column].rgbtRed   = round(tempRed);
        filter[0][column].rgbtGreen = round(tempGreen);
        filter[0][column].rgbtBlue  = round(tempBlue);
        tempRed = 0.0;
        tempGreen = 0.0;
        tempBlue = 0.0;
    }

    //top row, last column
    //do manually again
    filter[0][width - 1].rgbtRed   = round((float)(image[0][width - 2].rgbtRed + image[0][width - 1].rgbtRed + image[1][width - 2].rgbtRed + image[1][width - 1].rgbtRed) / 4);
    filter[0][width - 1].rgbtGreen = round((float)(image[0][width - 2].rgbtGreen + image[0][width - 1].rgbtGreen + image[1][width - 2].rgbtGreen + image[1][width - 1].rgbtGreen) / 4);
    filter[0][width - 1].rgbtBlue  = round((float)(image[0][width - 2].rgbtBlue + image[0][width - 1].rgbtBlue + image[1][width - 2].rgbtBlue + image[1][width - 1].rgbtBlue) / 4);

    //all middle pixels
    //scan all the pixels above, below and right and add to float, then round float and add to filter
    for (int row = 1, h = height - 1; row < h; row++)
    {
        //first column, middle rows
        for (int r = -1; r < 2; r++)
        {
            for (int c = 0; c < 2; c++)
            {
                tempRed   += (float) image[row + r][0 + c].rgbtRed / 6;
                tempGreen += (float) image[row + r][0 + c].rgbtGreen / 6;
                tempBlue  += (float) image[row + r][0 + c].rgbtBlue / 6;
            }
        }
        filter[row][0].rgbtRed   += round(tempRed);
        filter[row][0].rgbtGreen += round(tempGreen);
        filter[row][0].rgbtBlue  += round(tempBlue);
        tempRed = 0.0;
        tempGreen = 0.0;
        tempBlue = 0.0;

        //middle columns, middle rows
        //scan all the pixels around it and add to float, then round float and add to filter
        for (int column = 1, w = width - 1; column < w; column ++)
        {
            for (int r = -1; r < 2; r++)
            {
                for (int c = -1; c < 2; c++)
                {
                    tempRed   += (float) image[row + r][column + c].rgbtRed / 9;
                    tempGreen += (float) image[row + r][column + c].rgbtGreen / 9;
                    tempBlue  += (float) image[row + r][column + c].rgbtBlue / 9;
                }
            }
            filter[row][column].rgbtRed   += round(tempRed);
            filter[row][column].rgbtGreen += round(tempGreen);
            filter[row][column].rgbtBlue  += round(tempBlue);
            tempRed = 0.0;
            tempGreen = 0.0;
            tempBlue = 0.0;
        }

        //last column, middle rows
        //average all above, below and left
        for (int r = -1; r < 2; r++)
        {
            for (int c = -1; c < 1; c++)
            {
                tempRed   += (float) image[row + r][width - 1 + c].rgbtRed / 6;
                tempGreen += (float) image[row + r][width - 1 + c].rgbtGreen / 6;
                tempBlue  += (float) image[row + r][width - 1 + c].rgbtBlue / 6;
            }
        }
        filter[row][width - 1].rgbtRed   += round(tempRed);
        filter[row][width - 1].rgbtGreen += round(tempGreen);
        filter[row][width - 1].rgbtBlue  += round(tempBlue);
        tempRed = 0.0;
        tempGreen = 0.0;
        tempBlue = 0.0;
    }
    //Last row, first column
    //do it manually
    filter[height - 1][0].rgbtRed   = round((float)(image[height - 2][0].rgbtRed + image[height - 2][1].rgbtRed + image[height - 1][0].rgbtRed + image[height - 1][1].rgbtRed) / 4);
    filter[height - 1][0].rgbtGreen = round((float)(image[height - 2][0].rgbtGreen + image[height - 2][1].rgbtGreen + image[height - 1][0].rgbtGreen + image[height - 1][1].rgbtGreen) / 4);
    filter[height - 1][0].rgbtBlue  = round((float)(image[height - 2][0].rgbtBlue + image[height - 2][1].rgbtBlue + image[height - 1][0].rgbtBlue + image[height - 1][1].rgbtBlue) / 4);

    //last row, middle columns
    //average all left, above and right
    for (int column = 1, w = width - 1; column < w; column++)
    {
        for (int r = -1; r < 1; r++)
        {
            for (int c = -1; c < 2; c++)
            {
                tempRed   += (float) image[height - 1 + r][column + c].rgbtRed / 6;
                tempGreen += (float) image[height - 1 + r][column + c].rgbtGreen / 6;
                tempBlue  += (float) image[height - 1 + r][column + c].rgbtBlue / 6;
            }
        }
        filter[height - 1][column].rgbtRed   += round(tempRed);
        filter[height - 1][column].rgbtGreen += round(tempGreen);
        filter[height - 1][column].rgbtBlue  += round(tempBlue);
        tempRed = 0.0;
        tempGreen = 0.0;
        tempBlue = 0.0;
    }

    //last row, last column
    //do it manually
    filter[height - 1][width - 1].rgbtRed   = round((float)(image[height - 2][width - 2].rgbtRed + image[height - 2][width - 1].rgbtRed + image[height - 1][width - 2].rgbtRed + image[height - 1][width - 1].rgbtRed) / 4);
    filter[height - 1][width - 1].rgbtGreen = round((float)(image[height - 2][width - 2].rgbtGreen + image[height - 2][width - 1].rgbtGreen + image[height - 1][width - 2].rgbtGreen + image[height - 1][width - 1].rgbtGreen) / 4);
    filter[height - 1][width - 1].rgbtBlue  = round((float)(image[height - 2][width - 2].rgbtBlue + image[height - 2][width - 1].rgbtBlue + image[height - 1][width - 2].rgbtBlue + image[height - 1][width - 1].rgbtBlue) / 4);

    //save new picture over old
    for (int row = 0; row < height; row++)
    {
        for (int column = 0, w = width; column < w; column ++)
        {
            image[row][column].rgbtRed   = filter[row][column].rgbtRed;
            image[row][column].rgbtGreen = filter[row][column].rgbtGreen;
            image[row][column].rgbtBlue  = filter[row][column].rgbtBlue;
        }
    }
    return;
}

