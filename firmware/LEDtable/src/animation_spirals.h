#ifndef SRC_ANIMATION_SPIRALS_H_
#define SRC_ANIMATION_SPIRALS_H_
#include "animation.h"
#include "Spiral.h"

class SpiralsAnimation : public Animation
{
   Spiral string1;
   Spiral string2;
   Spiral string3;
   Spiral string4;

public:
   SpiralsAnimation(PixelDisplay *display);
   virtual ~SpiralsAnimation();

   void run();
};

#endif /* SRC_ANIMATION_SPIRALS_H_ */
