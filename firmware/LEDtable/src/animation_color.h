#ifndef SRC_ANIMATION_COLOR_H_
#define SRC_ANIMATION_COLOR_H_
#include "animation.h"

class ColorAnimation : public Animation
{
   int mColor;
public:
   ColorAnimation(PixelDisplay *display);
   virtual ~ColorAnimation();

   void run();
};

#endif /* SRC_ANIMATION_COLOR_H_ */
