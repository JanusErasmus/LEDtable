#ifndef SRC_ANIMATION_H_
#define SRC_ANIMATION_H_
#include "pixel_display.h"

class Animation
{
protected:
   PixelDisplay *mDisplay;
public:
   Animation(PixelDisplay *display){ mDisplay = display; };
   virtual ~Animation(){};
   virtual void run() = 0;
};



#endif /* SRC_ANIMATION_H_ */
