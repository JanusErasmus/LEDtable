#ifndef SRC_CHAR_PRINTER_H_
#define SRC_CHAR_PRINTER_H_
#include "animation.h"
#include "pixel_display.h"

class CharObject
{
public:
   int width;
   cyg_uint8 *map;
   CharObject *next;

   CharObject(const char c);
   ~CharObject();

   void fillFrame(cyg_uint16 *frame);

   void print();
};

class CharPrinter : public Animation
{
   cRGB mFont;
   cRGB mBackground;
   cyg_uint16 *mFrame;
   int mStringWidth;
   int mCurrrentShift;

public:
   CharPrinter(PixelDisplay *display);
   virtual ~CharPrinter();

   bool setString(const char *string, cRGB font, cRGB back);

   void run();
};

#endif /* SRC_CHAR_PRINTER_H_ */
