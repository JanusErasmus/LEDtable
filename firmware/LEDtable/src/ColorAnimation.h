#ifndef SRC_COLORANIMATION_H_
#define SRC_COLORANIMATION_H_
#include "Animation.h"

class ColorAnimation : public Animation
{
   int mColor;
public:
   ColorAnimation();
   virtual ~ColorAnimation();

   void run();
};

#endif /* SRC_COLORANIMATION_H_ */
