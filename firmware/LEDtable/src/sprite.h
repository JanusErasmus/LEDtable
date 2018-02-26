#ifndef LEDTABLE_SRC_SPRITE_H_
#define LEDTABLE_SRC_SPRITE_H_
#include "rgb.h"

class Sprite
{
public:
   int x;
   int y;
   int w;
   int h;

   cRGB **map;

   Sprite();
   Sprite(
         int x,
         int y,
         int w,
         int h
   );

   void print();

   virtual ~Sprite();
};

#endif /* LEDTABLE_SRC_SPRITE_H_ */
