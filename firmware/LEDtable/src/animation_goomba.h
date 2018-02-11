#ifndef SRC_ANIMATION_GOOMBA_H_
#define SRC_ANIMATION_GOOMBA_H_
#include <cyg/kernel/kapi.h>

#include "animation.h"

class GoombaAnimation : public Animation
{
   cyg_uint32 mAddress;

public:
   GoombaAnimation(PixelDisplay *display);
   virtual ~GoombaAnimation();

   void run();
};

#endif /* SRC_ANIMATION_GOOMBA_H_ */
