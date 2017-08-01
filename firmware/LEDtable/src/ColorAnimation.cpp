#include "ColorAnimation.h"
#include "ws281x_driver.h"

ColorAnimation::ColorAnimation()
{
}

void ColorAnimation::run()
{
   cWS281xDriver::get()->setAll(red);
   cWS281xDriver::get()->paint();
   cyg_thread_delay(100);
   cWS281xDriver::get()->setAll(green);
   cWS281xDriver::get()->paint();
   cyg_thread_delay(100);
   cWS281xDriver::get()->setAll(blue);
   cWS281xDriver::get()->paint();
   cyg_thread_delay(100);
   cWS281xDriver::get()->setAll(white);
   cWS281xDriver::get()->paint();
   cyg_thread_delay(100);
}

ColorAnimation::~ColorAnimation()
{
}

