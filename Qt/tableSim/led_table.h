#ifndef LEDTABLE_H
#define LEDTABLE_H
#include "pixel.h"

#define PIXEL_SIZE 25

class LEDtable
{
    QList<Pixel*> mTable;

public:
    LEDtable(int xCount, int yCount);

    void clear();
    void paint(QPainter *);
    void setPixel(int x, int y, QColor color);
};

#endif // LEDTABLE_H
