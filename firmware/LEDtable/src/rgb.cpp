#include "rgb.h"



cRGB::cRGB(cyg_uint8 r, cyg_uint8 g, cyg_uint8 b) :
    R(r), G(g), B(b)
{
}

cRGB off(0x0,0x0,0x0);
cRGB white(255,255,255);
cRGB blue(0x00, 0x00, 0xFF);
cRGB red(0xFF, 0x00, 0x00);
cRGB green(0x00, 0xFF, 0x00);
cRGB mix1(0xFF, 0xFF, 0x00);
cRGB mix2(0xFF, 0x00, 0xFF);
cRGB mix3(0x00, 0xFF, 0xFF);
cRGB gray(0x40, 0x40, 0x40);
cRGB orange(179, 89, 0);
cRGB pink(159, 0, 80);

cRGB lightBlue(128, 201, 201);
cRGB brown(64, 16, 0);
cRGB lightBrown(200, 50, 10);

cRGB *pColor[] = {
      &blue,
      &green,
      &red,
      &off,
      &brown,
      &gray,
      &lightBrown,
      &white,
      &orange,
      &pink,
      &mix1,
      &mix2,
      &mix3,
      &lightBlue,
      0
};
