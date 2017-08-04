#include "BigSpiralAnimation.h"
#include "ws281x_driver.h"

BigSpiralAnimation::BigSpiralAnimation() : string1(15, 0 , 15, 15, 0 , 0)
{
}

void BigSpiralAnimation::run()
{
   string1.run();
   cWS281xDriver::get()->paint();
   string1.next();
   cyg_thread_delay(1);
}

BigSpiralAnimation::~BigSpiralAnimation()
{
}

