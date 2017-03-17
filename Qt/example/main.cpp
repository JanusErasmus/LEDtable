#include <QCoreApplication>


#include <stdio.h>
#include <stdint.h>

#include "rgb_pixel_row.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    printf("interactiveLED\n");

    cPixelRow rowBuffer[2];

    cRGB frameBuffer[2][8];

    frameBuffer[0][0] = /*cRGB(3,4,5);*/
    frameBuffer[1][0] = /*cRGB(2,3,4);*/
    frameBuffer[2][0] = cRGB(5,6,7);

    cPixelRow::fillPixelRows(rowBuffer, frameBuffer, 2);
    //rowBuffer[0].setPixel(frameBuffer[0][0], 0);

    for(uint8_t k = 0; k < 2; k++)
    {
        rowBuffer[k].print();
    }

    uint8_t *dma = (uint8_t*)rowBuffer;
    for(uint8_t k = 0; k < (2 * sizeof(cPixelRow)); k++)
    {
        printf("%2d: %02X\n", k, dma[k]);

        if(k && !((k + 1) % 8))
            printf("\n");
    }
    return a.exec();
}
