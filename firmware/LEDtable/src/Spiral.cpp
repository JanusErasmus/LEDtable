#include "Spiral.h"
#include "ws281x_driver.h"


static Direction DirectionUp = Direction(0,1);
static Direction DirectionDown = Direction(0,-1);
static Direction DirectionLeft = Direction(1,0);
static Direction DirectionRight = Direction(-1,0);
static Direction *Directions[] = {&DirectionUp, &DirectionRight, &DirectionDown, &DirectionLeft, 0};

Spiral::Spiral(cyg_uint8 Xstart, cyg_uint8 Ystart, cyg_uint8 XtopMargin, cyg_uint8 YtopMargin, cyg_uint8 XbottomMargin, cyg_uint8 YbottomMargin)
{
   mXtopMargin = XtopMargin;
   mYtopMargin = YtopMargin;
   mXbottomMargin = XbottomMargin;
   mYbottomMargin = YbottomMargin;

   mXtopLimit = mXtopMargin;
   mYtopLimit = mYtopMargin;
   mXbottomLimit = mXbottomMargin;
   mYbottomLimit = mYbottomMargin;

   mDirection = Directions[0];

   mXstart = Xstart;
   mYstart = Ystart;

   mX = mXstart;
   mY = mYstart;

   cDirection = 0;
   cColor = 1;
   mDirDiff = 1;

}

void Spiral::run()
{
   cWS281xDriver::get()->setPixel(mX, mY, *pColor[cColor]);
}

cyg_bool Spiral::collide(cyg_uint8 currentX, cyg_uint8 currentY, Direction *direction)
{
   //Check X
   cyg_int8 x = currentX + direction->mXdiff;
   if((mXbottomLimit > x) || (x > mXtopLimit))
      return true;

   //check y
   cyg_int8 y = currentY + direction->mYdiff;
   if((mYbottomLimit > y) || (y > mYtopLimit))
      return true;


   return false;
}

cyg_bool Spiral::move()
{
   if(!collide(mX, mY, mDirection))
   {
      mX += mDirection->mXdiff;
      mY += mDirection->mYdiff;

      return true;
   }

   return false;
}

void Spiral::next()
{
   cyg_bool resetMove = false;
   while(!move())
   {
      //decrease left margin
      if(mDirection == &DirectionUp)
      {
         mXtopLimit--;

      }

      //decrease right margin
      if(mDirection == &DirectionDown)
      {
         mXbottomLimit++;
      }

      //decrease bottom
      if(mDirection == &DirectionLeft)
      {
         mYbottomLimit++;
      }

      //decrease top
      if(mDirection == &DirectionRight)
      {
         mYtopLimit--;

      }

      cDirection += mDirDiff;

      if(cDirection < 0)
         cDirection = 3;

      if(!Directions[cDirection])
      {
         cDirection = 0;
      }

      mDirection = Directions[cDirection];

//      diag_printf("xTop %d, xBot %d, yTop %d, yBot %d\n", mXtopLimit, mYbottomLimit, mYtopLimit, mYbottomLimit);
      if((mYtopLimit < mYbottomLimit) || (mXtopLimit < mXbottomLimit))
      {
         resetMove = true;
         break;
      }
   }

   if(resetMove)
   {
      mXtopLimit = mXtopMargin;
      mYtopLimit = mYtopMargin;
      mXbottomLimit = mXbottomMargin;
      mYbottomLimit = mYbottomMargin;

      cDirection = 0;
      mDirection = Directions[0];

      mX = mXstart;
      mY = mYstart;

      cColor++;
      if(!pColor[cColor])
            cColor = 0;
   }

//   diag_printf("%d, %d\n", mX, mY);

}

Spiral::~Spiral()
{
   // TODO Auto-generated destructor stub
}

