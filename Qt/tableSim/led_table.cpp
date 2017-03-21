#include "led_table.h"

LEDtable::LEDtable(int xCount, int yCount)
{
    int x = 10;
    int y = 20;
    for (int k = 0; k < xCount; ++k)
    {
        for (int k = 0; k < yCount; ++k)
        {
            Pixel *rect = new Pixel(x, y, PIXEL_SIZE, PIXEL_SIZE);
            x += PIXEL_SIZE;
            mTable.append(rect);
        }
        y += PIXEL_SIZE;
        x = 10;
    }
}

void LEDtable::clear()
{
    foreach(Pixel *p, mTable)
        p->setColor(Qt::darkGray);
}

void LEDtable::setPixel(int x, int y, QColor color)
{
    int pos = (y * 16) + x;
    if(pos < mTable.length())
    {
        mTable.at(pos)->setColor(color);
    }
}

void LEDtable::paint(QPainter *painter)
{
    foreach (Pixel *p, mTable)
    {
        p->paint(painter);
    }
}
