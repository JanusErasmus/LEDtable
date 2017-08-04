#ifndef SRC_BIGSPIRALANIMATION_H_
#define SRC_BIGSPIRALANIMATION_H_
#include "Animation.h"
#include "Spiral.h"

class BigSpiralAnimation : public Animation
{
   Spiral string1;

public:
   BigSpiralAnimation();
   virtual ~BigSpiralAnimation();

   void run();
};


#endif /* SRC_BIGSPIRALANIMATION_H_ */
