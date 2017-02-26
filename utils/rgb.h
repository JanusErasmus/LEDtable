#ifndef CRGB_H
#define CRGB_H
#include <stdint.h>

class cRGB
{
public:
    uint8_t R;
    uint8_t G;
    uint8_t B;

    cRGB(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0);
};

#endif // CRGB_H
