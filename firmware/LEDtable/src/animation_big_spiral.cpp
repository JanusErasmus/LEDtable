#include "animation_big_spiral.h"

BigSpiralAnimation::BigSpiralAnimation(PixelDisplay *display) : Animation(display), string1(display, 15, 0 , 15, 15, 0 , 0)
{
}

void BigSpiralAnimation::run()
{
   string1.run();
   mDisplay->paint();
   string1.next();
   cyg_thread_delay(1);
}

BigSpiralAnimation::~BigSpiralAnimation()
{
}

