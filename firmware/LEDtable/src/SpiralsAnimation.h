#ifndef SRC_SPIRALSANIMATION_H_
#define SRC_SPIRALSANIMATION_H_
#include "Animation.h"
#include "Spiral.h"

class SpiralsAnimation : public Animation
{
   Spiral string1;
   Spiral string2;
   Spiral string3;
   Spiral string4;

public:
   SpiralsAnimation();
   virtual ~SpiralsAnimation();

   void run();
};

#endif /* SRC_SPIRALSANIMATION_H_ */
