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

extern cRGB *pColor[];

#endif // CRGB_H
