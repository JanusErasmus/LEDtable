#ifndef SRC_RUNNERANIMATION_H_
#define SRC_RUNNERANIMATION_H_
#include "Animation.h"
#include "Runner.h"

class RunnerAnimation : public Animation
{
    Runner mRunner;

public:
   RunnerAnimation();
   virtual ~RunnerAnimation();

   void run();
};

#endif /* SRC_RUNNERANIMATION_H_ */
