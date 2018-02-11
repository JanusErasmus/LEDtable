#include "animation_runner.h"

RunnerAnimation::RunnerAnimation(PixelDisplay *display) : Animation(display)
{
}

void RunnerAnimation::run()
{
   mRunner.run();
   mRunner.next();
   mDisplay->paint();
   cyg_thread_delay(2);
}

RunnerAnimation::~RunnerAnimation()
{
}

