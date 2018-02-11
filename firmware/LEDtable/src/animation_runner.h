#ifndef SRC_ANIMATION_RUNNER_H_
#define SRC_ANIMATION_RUNNER_H_
#include "animation.h"
#include "Runner.h"

class RunnerAnimation : public Animation
{
    Runner mRunner;

public:
   RunnerAnimation(PixelDisplay *display);
   virtual ~RunnerAnimation();

   void run();
};

#endif /* SRC_ANIMATION_RUNNER_H_ */
