#ifndef CLOCK_H
#define CLOCK_H
#include <QColor>

#include "led_table.h"

class Clock
{
    LEDtable *mTable;

    void redrawClock();
    void drawArm(int armPos, int length, QColor color);
    void drawLine(double theta, int length, QColor color);

public:
    Clock(LEDtable *table);

    void update();
};

#endif // CLOCK_H
