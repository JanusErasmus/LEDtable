#include "RunnerAnimation.h"
#include "ws281x_driver.h"

RunnerAnimation::RunnerAnimation()
{
}

void RunnerAnimation::run()
{
   mRunner.run();
   cWS281xDriver::get()->paint();
   mRunner.next();
}

RunnerAnimation::~RunnerAnimation() {
   // TODO Auto-generated destructor stub
}

