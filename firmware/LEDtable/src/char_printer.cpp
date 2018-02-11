#include <cyg/kernel/diag.h>
#include <stdlib.h>
#include <string.h>

#include "courier.h"
#include "char_printer.h"

CharObject::CharObject(const char c)
{
   int mOffsetMultiplier = courier[1] + 3;

   int offset = (((unsigned int)c - 32) * mOffsetMultiplier) + 2;
   this->width = courier[offset] + 1;
   this->map = (cyg_uint8*)malloc(courier[1]);
   memcpy(this->map, (cyg_uint8*)&courier[offset + 3], courier[1]);
   next = 0;
}

CharObject::~CharObject()
{
   free(this->map);
}

void CharObject::fillFrame(cyg_uint16 *frame)
{
   for(int column = 0; column < width; column++)
   {
      for(int row = 0; row < courier[1]; row++)
      {
         if(map[row] & (1 << (7 - column)))
            frame[column] |= (1 << row);
      }
   }
}

void CharObject::print()
{
   diag_printf("\n");
   for(int row = 0; row < courier[1]; row++)
   {
      for(int column = 0; column < width; column++)
      {
         if(map[row] & (1 << (7 - column)))
            diag_printf("#");
         else
            diag_printf(" ");
      }
      diag_printf("\n");
   }
}

CharPrinter::CharPrinter(PixelDisplay *display) : mDisplay(display)
{
   mFrame = 0;
   mCurrrentShift = 0;
   mStringWidth = 0;
   mFont = yellow;
   mBackground = off;
}

bool CharPrinter::setString(const char *string, cRGB font, cRGB back)
{
   CharObject *headObject;

   if(!string || !string[0])
      return false;

   mFont = font;
   mBackground = back;

   headObject = new CharObject(string[0]);

   CharObject *o = headObject;
   int k = 0;
   while(string[++k])
   {
      //o->print();
      o->next = new CharObject(string[k]);
      o = o->next;
   }

   mStringWidth = 0;
   o = headObject;
   while(o)
   {
      mStringWidth += o->width;
      o = o->next;
   };

   if(mFrame)
      free(mFrame);

   //diag_printf("totalBuffer %d\n", mStringWidth);
   mFrame = (cyg_uint16*)malloc(mStringWidth * 2);
   memset(mFrame, 0, mStringWidth * 2);

   int offset = 0;
   o = headObject;
   while(o)
   {
      o->fillFrame(&mFrame[offset]);
      offset += o->width;

      CharObject *d = o;
      o = o->next;
      delete d;
   };

   return true;
}

void CharPrinter::run()
{
   int viewStart = mCurrrentShift;
   int viewEnd = viewStart + 16;
   if(viewEnd > mStringWidth)
      viewEnd = mStringWidth;

   int x = 0;
   for(int column = viewStart; column < viewEnd; column++)
   {
      int y = 2;
      for(int row = 0; row < courier[1]; row++)
      {
         if(mFrame[column] & (1 << row))
            mDisplay->setPixel(x, y, mFont);
         else
            mDisplay->setPixel(x, y, mBackground);

         y++;
      }
      x++;
   }
   mCurrrentShift++;
   if(mCurrrentShift > mStringWidth)
      mCurrrentShift = 0;

   mDisplay->paint();
}
CharPrinter::~CharPrinter()
{
   if(mFrame)
      free(mFrame);
}

