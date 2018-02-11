#include <stdlib.h>

#include "animation_color.h"

ColorAnimation::ColorAnimation(PixelDisplay *display) : Animation(display)
{
   mColor = 0;
}

void ColorAnimation::run()
{
   cyg_uint8 prevColor = mColor;
   mColor = rand() % 20;
   while((mColor == prevColor) && (mColor > PCOLOR_COUNT))
         mColor = rand() % 20;

   mDisplay->setAll(*pColor[mColor]);
   mDisplay->paint();
   cyg_thread_delay(100);
}

ColorAnimation::~ColorAnimation()
{
}

