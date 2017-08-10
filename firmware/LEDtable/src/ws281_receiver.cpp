#include "ws281_receiver.h"
#include "ws281x_driver.h"

#define TRACE(_x, ...) INFO_TRACE("WS281rx", _x,  ##__VA_ARGS__)

WS281receiver::WS281receiver() : cHDLCreceiver(4096)
{
}

void WS281receiver::handleData(cyg_uint8 *buff, cyg_uint32 len)
{
   TRACE("Draw %d bytes, %d pixels\n", len, (len/3));
   //diag_dump_buf(buff, len);

   cyg_uint8 x = 0;
   cyg_uint8 y = 0;
   cyg_uint32 index = 0;
   for(cyg_uint32 k = 0; k < (len / 3); k++)
   {
      cRGB color(buff[index + 2], buff[index + 1], buff[index]);
      index += 3;

      //diag_printf("set (%d,%d)\n", x++, y);
      cWS281xDriver::get()->setPixel(x++, y, color);
      if(x && ((x % 16) == 0))
      {
         x = 0;
         y++;
         if(y && ((y % 16) == 0))
            break;
      }
   }

   cWS281xDriver::get()->paint();
}

WS281receiver::~WS281receiver()
{
}

