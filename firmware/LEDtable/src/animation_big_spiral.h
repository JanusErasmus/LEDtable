#ifndef SRC_ANIMATION_BIG_SPIRAL_H_
#define SRC_ANIMATION_BIG_SPIRAL_H_
#include "animation.h"
#include "Spiral.h"

class BigSpiralAnimation : public Animation
{
   Spiral string1;

public:
   BigSpiralAnimation(PixelDisplay *display);
   virtual ~BigSpiralAnimation();

   void run();
};


#endif /* SRC_ANIMATION_BIG_SPIRAL_H_ */
