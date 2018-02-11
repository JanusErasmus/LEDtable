#include <stdlib.h>

#include "ColorAnimation.h"
#include "ws281x_driver.h"
#include "rgb.h"

ColorAnimation::ColorAnimation()
{
   mColor = 0;
}

void ColorAnimation::run()
{
   cyg_uint8 prevColor = mColor;
   mColor = rand() % 20;
   while((mColor == prevColor) && (mColor > PCOLOR_COUNT))
         mColor = rand() % 20;

   cWS281xDriver::get()->setAll(*pColor[mColor]);
   cWS281xDriver::get()->paint();
   cyg_thread_delay(100);
}

ColorAnimation::~ColorAnimation()
{
}

