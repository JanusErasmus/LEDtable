#include <string.h>
#include <stdio.h>

#include "rgb_pixel_row.h"

cPixelRow::cPixelRow()
{
    memset(R, 0, 8);
    memset(G, 0, 8);
    memset(B, 0, 8);
}

void cPixelRow::setColor(uint8_t *color, uint8_t pixel_byte, uint8_t column)
{
    column = 1 << column;
    uint8_t bit = 1;
    for(uint8_t k = 0; k < 8; k++)
    {
        if(pixel_byte & bit)
            color[k] |= column;
        else
            color[k] &= ~(column);

        bit <<= 1;
    }
}

void cPixelRow::setPixel(cRGB pixel, uint8_t column)
{
    setColor(R, pixel.R, column);
    setColor(G, pixel.G, column);
    setColor(B, pixel.B, column);
}

void cPixelRow::fillPixelRows(cPixelRow *row, cRGB pixels[][8], int rows)
{
    for(uint8_t k = 0; k < rows; k++)
    {
        for(uint8_t column = 0; column < 8; column++)
        {
            row[k].setPixel(pixels[k][column], column);
        }
    }
}

void cPixelRow::printRow(const char *name, uint8_t *row)
{
    for(uint8_t k = 0; k < 8; k++)
    {
        printf("%5s%d: ", name, k);

        for(uint8_t i = 0; i < 8; i++)
        {
            if(row[k] & (1 << i))
                printf("1");
            else
                printf("0");
        }

        printf("\n");
    }
}

void cPixelRow::print()
{
    printf("%5s   01234567\n", "");
    printRow("R", R);
    printRow("G", G);
    printRow("B", B);
}
