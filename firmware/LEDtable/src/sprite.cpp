#include <cyg/kernel/diag.h>
#include <stdlib.h>
#include <string.h>

#include "sprite.h"

Sprite::Sprite()
{
   x = 0;
   y = 0;
   w = 0;
   h = 0;
   map = 0;
}

Sprite::Sprite(
   int x,
   int y,
   int w,
   int h
) :
      x(x),
      y(y),
      w(w),
      h(h)
{
   map = (cRGB**)malloc(w * sizeof(cRGB*));

   for(int k= 0; k < 2; k++)
   {
      map[k] = (cRGB*)malloc(h * sizeof(cRGB));
      memset(map[k], 0 ,h * sizeof(cRGB));
   }
}

void Sprite::print()
{
   diag_printf("x: %d\n",x);
   diag_printf("y: %d\n",y);
   diag_printf("w: %d\n",w);
   diag_printf("h: %d\n",h);
   for(int c = 0; c < w; c++)
   {
      for(int r = 0; r < h; r++)
      {
         cRGB p = map[c][r];
         diag_printf("(%d,%d)[%02X, %02X, %02X]\n", c, r, p.R, p.G, p.B);
      }
   }
}

Sprite::~Sprite()
{
   for(int k= 0; k < 2; k++)
   {
      free(map[k]);
   }

   free(map);
}

