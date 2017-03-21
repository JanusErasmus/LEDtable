#include"pixel.h"

Pixel::Pixel(double x, double y, int w, int l) : QObject(0), xPos(x), yPos(y), width(w),  length(l)
{
    mColor = Qt::darkGray;
}

void Pixel::paint(QPainter * p)
{
    p->setBrush(mColor);
    p->drawRect(xPos, yPos, width, length);
}

