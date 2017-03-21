#include <QTime>

#define _USE_MATH_DEFINES
#include <math.h>

#include "clock.h"

Clock::Clock(LEDtable *table) : mTable(table)
{
}

int positions[] =
{
    0,
    -345,
    -330,
    -315,
    -300,
    -285,
    -270,
    -255,
    -240,
    -225,
    -210,
    -195,
    180,
    195,
    210,
    225,
    240,
    255,
    270,
    285,
    300,
    315,
    330,
    345,
    360
};

void Clock::drawArm(int armPos, int length, QColor color)
{
    int degrees = (armPos * 6) - 90;
    if(degrees <= 0)
        degrees = 360 + degrees;

    int idx = (degrees / 15);

    drawLine(positions[idx], length, color);
}
void Clock::drawLine(double theta, int length, QColor color)
{
    for(int radius = 0; radius < length; radius++)
    {
        int y = sin(theta * ((double)M_PI / 180.0)) * radius;
        int x = cos(theta * ((double)M_PI / 180.0)) * radius;

        mTable->setPixel(x + 7, y + 8, color);
    }
}

void Clock::redrawClock()
{
    mTable->clear();

    mTable->setPixel(7, 1, Qt::blue);
    mTable->setPixel(3, 2, Qt::blue);
    mTable->setPixel(1, 4, Qt::blue);

    mTable->setPixel(11, 2, Qt::blue);
    mTable->setPixel(13, 4, Qt::blue);
    mTable->setPixel(14, 8, Qt::blue);

    mTable->setPixel(7, 15, Qt::blue);
    mTable->setPixel(3, 14, Qt::blue);
    mTable->setPixel(1, 12, Qt::blue);
    mTable->setPixel(0, 8, Qt::blue);

    mTable->setPixel(11, 14, Qt::blue);
    mTable->setPixel(13, 12, Qt::blue);
    mTable->setPixel(14, 8, Qt::blue);
}

void Clock::update()
{
    QTime currenttime = QTime::currentTime();

    redrawClock();
    drawArm(currenttime.second(), 7, Qt::lightGray);
    drawArm(currenttime.minute(), 7, Qt::white);

    int hour = currenttime.hour();
    if(hour > 12)
        hour = hour - 12;

    drawArm(hour * 5, 6, Qt::gray);
}
