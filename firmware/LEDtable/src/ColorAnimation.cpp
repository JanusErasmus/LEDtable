#include "ColorAnimation.h"
#include "ws281x_driver.h"

ColorAnimation::ColorAnimation()
{
   mColor = 0;
}

void ColorAnimation::run()
{
   mColor++;
   if(!pColor[mColor])
         mColor = 0;

   cWS281xDriver::get()->setAll(*pColor[mColor]);
   cWS281xDriver::get()->paint();
//   cyg_thread_delay(100);
//   cWS281xDriver::get()->setAll(green);
//   cWS281xDriver::get()->paint();
//   cyg_thread_delay(100);
//   cWS281xDriver::get()->setAll(blue);
//   cWS281xDriver::get()->paint();
//   cyg_thread_delay(100);
//   cWS281xDriver::get()->setAll(white);
//   cWS281xDriver::get()->paint();
   cyg_thread_delay(100);
}

ColorAnimation::~ColorAnimation()
{
}

