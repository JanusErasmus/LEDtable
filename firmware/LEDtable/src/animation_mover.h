#ifndef LEDTABLE_SRC_ANIMATION_MOVER_H_
#define LEDTABLE_SRC_ANIMATION_MOVER_H_
#include <cyg/kernel/kapi.h>

#include "blue_device.h"
#include "animation.h"
#include "sprite.h"

class MoverAnimation : public cButtonHandler, public Animation
{
   cyg_mutex_t mUpdateMutex;
   bool mUpdate;
   Sprite *mSprite;

public:
   MoverAnimation(PixelDisplay *display);
   virtual ~MoverAnimation();

   void run();

   void handleButton(eButton button);
};

#endif /* LEDTABLE_SRC_ANIMATION_MOVER_H_ */
