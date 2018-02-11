#include "rgb.h"



cRGB::cRGB(cyg_uint8 r, cyg_uint8 g, cyg_uint8 b) :
    R(r), G(g), B(b)
{
}

cRGB off(0x0,0x0,0x0);
cRGB white(255,255,255);
cRGB red(0xFF, 0, 0);
cRGB darkred(0x80, 0, 0);
cRGB orange(0xFF,50,0);
cRGB yellow(0xFF,0xFF,0);
cRGB lightGreen(0x8E, 0xFF, 0);
cRGB green(0x00, 0xFF, 0x00);
cRGB gray(0x40, 0x40, 0x40);
cRGB turkois(0, 0xFF, 0xFF);
cRGB lightBlue(0, 0xC0, 0xFF);
cRGB blue(0x00, 0x00, 0xFF);
cRGB violate(0x80, 0x00, 0xFF);
cRGB pink(0xFF, 0, 48);
cRGB brown(0x80, 0, 0);
cRGB lightBrown(200, 50, 10);


cRGB *pColor[] = {
      &red,
      &darkred,
      &orange,
      &yellow,
      &lightGreen,
      &green,
      &gray,
      &turkois,
      &lightBlue,
      &blue,
      &violate,
      &pink,
      &lightBlue,
      &brown,
      &lightBrown,
      0
};
