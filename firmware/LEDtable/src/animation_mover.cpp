#include <cyg/kernel/diag.h>
#include <stdlib.h>

#include "animation_mover.h"

MoverAnimation::MoverAnimation(PixelDisplay *display) : Animation(display)
{
   cyg_mutex_init(&mUpdateMutex);
   mUpdate = true;
   mSprite = new Sprite(7, 7, 2, 2);

   cRGB *p = &mSprite->map[0][0];
   p->R = 0xFF;
   p = &mSprite->map[0][1];
      p->G = 0xFF;
      p = &mSprite->map[1][0];
         p->B = 0xFF;
         p = &mSprite->map[1][1];
            p->B = 0xFF;
            p->G = 0xFF;
}

void MoverAnimation::run()
{
   cyg_mutex_lock(&mUpdateMutex);
   if(mUpdate)
   {
      mUpdate = false;

   mDisplay->setAll(off);

   if(mSprite)
   {
      for(int c = 0; c < mSprite->w; c++)
      {
         for(int r = 0; r < mSprite->h; r++)
         {
            mDisplay->setPixel(mSprite->x + c, mSprite->y + r, mSprite->map[c][r]);
         }

      }
   }
   mDisplay->paint();
   }
   cyg_mutex_unlock(&mUpdateMutex);
   cyg_thread_delay(10);
}

void MoverAnimation::handleButton(cButtonHandler::eButton button)
{
   cyg_mutex_lock(&mUpdateMutex);
   switch(button)
   {
   case UP:
//   diag_printf("U\n");
   mSprite->y++;
   mUpdate = true;
   break;
   case DOWN:
//   diag_printf("D\n");
   mSprite->y--;
   mUpdate = true;
   break;
   case LEFT:
      mSprite->x--;
      mUpdate = true;
//   diag_printf("L\n");
   break;
   case RIGHT:
      mSprite->x++;
      mUpdate = true;
//   diag_printf("R\n");
   break;
   }

//   mSprite->print();
   cyg_mutex_unlock(&mUpdateMutex);
}

MoverAnimation::~MoverAnimation()
{
   // TODO Auto-generated destructor stub
}

