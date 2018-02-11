#ifndef SRC_PIXEL_DISPLAY_H_
#define SRC_PIXEL_DISPLAY_H_
#include <cyg/kernel/kapi.h>

#include "rgb.h"

class PixelDisplay
{
public:
   virtual ~PixelDisplay(){};

   virtual void setPixel(cyg_uint8 x, cyg_uint8 y, cRGB color) = 0;
   virtual void setBuffer(cyg_uint8 *buffer, cyg_uint32 len) = 0;
   virtual void setAll(cRGB color) = 0;
   virtual void paint() = 0;
};



#endif /* SRC_PIXEL_DISPLAY_H_ */
