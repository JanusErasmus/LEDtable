#ifndef CRGBPIXELROW_H
#define CRGBPIXELROW_H
#include "rgb.h"

class cPixelRow
{
    void setColor(uint8_t *color, uint8_t pixel_byte, uint8_t column);

    void printRow(const char *name, uint8_t *row);

public:
    uint8_t R[8];
    uint8_t G[8];
    uint8_t B[8];

    cPixelRow();

    void setPixel(cRGB pixel, uint8_t column);

    static void fillPixelRows(cPixelRow *row, cRGB pixels[][8], int rows);

    void print();
};

#endif // CRGBPIXELROW_H
