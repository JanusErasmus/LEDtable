#ifndef SRC_GOOMBAANIMATION_H_
#define SRC_GOOMBAANIMATION_H_
#include <cyg/kernel/kapi.h>

#include "Animation.h"

class GoombaAnimation : public Animation
{
   cyg_uint32 mAddress;

public:
   GoombaAnimation();
   virtual ~GoombaAnimation();

   void run();
};

#endif /* SRC_GOOMBAANIMATION_H_ */
