#ifndef CRGB_H
#define CRGB_H
#include <cyg/kernel/kapi.h>


class cRGB
{
public:
    cyg_uint8 R;
    cyg_uint8 G;
    cyg_uint8 B;

    cRGB(cyg_uint8 r = 0, cyg_uint8 g = 0, cyg_uint8 b = 0);
};

#define PCOLOR_COUNT 14
extern cRGB *pColor[];

extern cRGB red;
extern cRGB darkred;
extern cRGB off;
extern cRGB green;
extern cRGB yellow;
extern cRGB blue;
extern cRGB white;
extern cRGB lightBlue;
extern cRGB brown;
extern cRGB lightBrown;

#endif // CRGB_H
