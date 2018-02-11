/*
 * Spiral.h
 *
 *  Created on: 09 Apr 2017
 *      Author: Janus
 */

#ifndef SRC_SPIRAL_H_
#define SRC_SPIRAL_H_
#include <cyg/kernel/kapi.h>
#include "pixel_display.h"

class Direction
{
public:
   cyg_int8 mXdiff;
   cyg_int8 mYdiff;

   Direction(cyg_int8 xDiff, cyg_int8 yDiff) : mXdiff(xDiff), mYdiff(yDiff){};
};

class Spiral
{
   PixelDisplay *mDisplay;

   cyg_uint8 mXstart, mYstart;
   cyg_int8 mXtopMargin, mYtopMargin, mXbottomMargin, mYbottomMargin;

   Direction *mDirection;
   cyg_int8 mXtopLimit, mYtopLimit, mXbottomLimit, mYbottomLimit;
   cyg_uint8 mX, mY;
   cyg_uint8 mColor;
   cyg_int8 cDirection;

   cyg_int8 mDirDiff;

   cyg_bool move();
   cyg_bool collide(cyg_uint8 currentX, cyg_uint8 currentY, Direction *direction);

public:
   Spiral(
         PixelDisplay *display,
         cyg_uint8 Xstart,
         cyg_uint8 Ystart,
         cyg_uint8 XtopMargin,
         cyg_uint8 YtopMargin,
         cyg_uint8 XbottomMargin,
         cyg_uint8 YbottomMargin);
   virtual ~Spiral();

   void run();
   void next();

   void startColor(cyg_uint8 index){ mColor = index; };
};


#endif /* SRC_SPIRAL_H_ */
