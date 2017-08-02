#include "RunnerAnimation.h"
#include "ws281x_driver.h"

RunnerAnimation::RunnerAnimation()
{
}

void RunnerAnimation::run()
{
   mRunner.run();
   mRunner.next();
   cWS281xDriver::get()->paint();
   cyg_thread_delay(2);
}

RunnerAnimation::~RunnerAnimation() {
   // TODO Auto-generated destructor stub
}

