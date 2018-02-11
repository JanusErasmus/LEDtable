#include "GoombaAnimation.h"
#include "ws281x_driver.h"
#include "spi_flash.h"

GoombaAnimation::GoombaAnimation()
{
   mAddress = 0xF0000;
}

void GoombaAnimation::run()
{
   cyg_uint8 buffer[1024];
   SpiFlash::get()->read(mAddress, buffer, 1024);
   cWS281xDriver::get()->setBuffer(buffer, 1024);
   cWS281xDriver::get()->paint();
   cyg_thread_delay(20);

   mAddress += 0x400;
   if(mAddress > 0xF7C00)
       mAddress = 0xF0000;
//   cWS281xDriver::get()->setAll(lightBlue);
//   cWS281xDriver::get()->setPixel(9 , 15, brown);
//   cWS281xDriver::get()->setPixel(8 , 15, brown);
//   cWS281xDriver::get()->setPixel(7 , 15, brown);
//   cWS281xDriver::get()->setPixel(6 , 15, brown);
//   cWS281xDriver::get()->setPixel(10, 14, brown);
//   cWS281xDriver::get()->setPixel( 9, 14, brown);
//   cWS281xDriver::get()->setPixel( 8, 14, brown);
//   cWS281xDriver::get()->setPixel( 7, 14, brown);
//   cWS281xDriver::get()->setPixel( 6, 14, brown);
//   cWS281xDriver::get()->setPixel( 5, 14, brown);
//   cWS281xDriver::get()->setPixel(11, 13, brown);
//   cWS281xDriver::get()->setPixel(10, 13, brown);
//   cWS281xDriver::get()->setPixel( 9, 13, brown);
//   cWS281xDriver::get()->setPixel( 8, 13, brown);
//   cWS281xDriver::get()->setPixel( 7, 13, brown);
//   cWS281xDriver::get()->setPixel( 6, 13, brown);
//   cWS281xDriver::get()->setPixel( 5, 13, brown);
//   cWS281xDriver::get()->setPixel( 4, 13, brown);
//   cWS281xDriver::get()->setPixel(12, 12, brown);
//   cWS281xDriver::get()->setPixel(11, 12, brown);
//   cWS281xDriver::get()->setPixel(10, 12, brown);
//   cWS281xDriver::get()->setPixel( 9, 12, brown);
//   cWS281xDriver::get()->setPixel( 8, 12, brown);
//   cWS281xDriver::get()->setPixel( 7, 12, brown);
//   cWS281xDriver::get()->setPixel( 6, 12, brown);
//   cWS281xDriver::get()->setPixel( 5, 12, brown);
//   cWS281xDriver::get()->setPixel( 4, 12, brown);
//   cWS281xDriver::get()->setPixel( 3, 12, brown);
//   cWS281xDriver::get()->setPixel(13, 11, brown);
//   cWS281xDriver::get()->setPixel(10, 11, brown);
//   cWS281xDriver::get()->setPixel( 9, 11, brown);
//   cWS281xDriver::get()->setPixel( 8, 11, brown);
//   cWS281xDriver::get()->setPixel( 7, 11, brown);
//   cWS281xDriver::get()->setPixel( 6, 11, brown);
//   cWS281xDriver::get()->setPixel( 5, 11, brown);
//   cWS281xDriver::get()->setPixel( 2, 11, brown);
//   cWS281xDriver::get()->setPixel(14, 10, brown);
//   cWS281xDriver::get()->setPixel(13, 10, brown);
//   cWS281xDriver::get()->setPixel(12, 10, brown);
//   cWS281xDriver::get()->setPixel( 9, 10, brown);
//   cWS281xDriver::get()->setPixel( 8, 10, brown);
//   cWS281xDriver::get()->setPixel( 7, 10, brown);
//   cWS281xDriver::get()->setPixel( 6, 10, brown);
//   cWS281xDriver::get()->setPixel( 3, 10, brown);
//   cWS281xDriver::get()->setPixel( 2, 10, brown);
//   cWS281xDriver::get()->setPixel( 1, 10, brown);
//   cWS281xDriver::get()->setPixel(14,  9, brown);
//   cWS281xDriver::get()->setPixel(13,  9, brown);
//   cWS281xDriver::get()->setPixel(12,  9, brown);
//   cWS281xDriver::get()->setPixel( 9,  9, brown);
//   cWS281xDriver::get()->setPixel( 8,  9, brown);
//   cWS281xDriver::get()->setPixel( 7,  9, brown);
//   cWS281xDriver::get()->setPixel( 6,  9, brown);
//   cWS281xDriver::get()->setPixel( 3,  9, brown);
//   cWS281xDriver::get()->setPixel( 2,  9, brown);
//   cWS281xDriver::get()->setPixel( 1,  9, brown);
//   cWS281xDriver::get()->setPixel(15,  8, brown);
//   cWS281xDriver::get()->setPixel(14,  8, brown);
//   cWS281xDriver::get()->setPixel(13,  8, brown);
//   cWS281xDriver::get()->setPixel(12,  8, brown);
//   cWS281xDriver::get()->setPixel( 9,  8, brown);
//   cWS281xDriver::get()->setPixel( 8,  8, brown);
//   cWS281xDriver::get()->setPixel( 7,  8, brown);
//   cWS281xDriver::get()->setPixel( 6,  8, brown);
//   cWS281xDriver::get()->setPixel( 3,  8, brown);
//   cWS281xDriver::get()->setPixel( 2,  8, brown);
//   cWS281xDriver::get()->setPixel( 1,  8, brown);
//   cWS281xDriver::get()->setPixel( 0,  8, brown);
//   cWS281xDriver::get()->setPixel(15,  7, brown);
//   cWS281xDriver::get()->setPixel(14,  7, brown);
//   cWS281xDriver::get()->setPixel(13,  7, brown);
//   cWS281xDriver::get()->setPixel(12,  7, brown);
//   cWS281xDriver::get()->setPixel( 9,  7, brown);
//   cWS281xDriver::get()->setPixel( 8,  7, brown);
//   cWS281xDriver::get()->setPixel( 7,  7, brown);
//   cWS281xDriver::get()->setPixel( 6,  7, brown);
//   cWS281xDriver::get()->setPixel( 3,  7, brown);
//   cWS281xDriver::get()->setPixel( 2,  7, brown);
//   cWS281xDriver::get()->setPixel( 1,  7, brown);
//   cWS281xDriver::get()->setPixel( 0,  7, brown);
//
//   for(int k = 0; k < 16; k++)
//      cWS281xDriver::get()->setPixel( k,  6, brown);
//
//
//
//   cWS281xDriver::get()->setPixel(11,  5, brown);
//   cWS281xDriver::get()->setPixel(12,  5, brown);
//   cWS281xDriver::get()->setPixel(13,  5, brown);
//   cWS281xDriver::get()->setPixel(14,  5, brown);
//   cWS281xDriver::get()->setPixel(10,  5, lightBrown);
//   cWS281xDriver::get()->setPixel( 9,  5, lightBrown);
//   cWS281xDriver::get()->setPixel( 8,  5, lightBrown);
//   cWS281xDriver::get()->setPixel( 7,  5, lightBrown);
//   cWS281xDriver::get()->setPixel( 6,  5, lightBrown);
//   cWS281xDriver::get()->setPixel( 5,  5, lightBrown);
//   cWS281xDriver::get()->setPixel( 4,  5, brown);
//   cWS281xDriver::get()->setPixel( 3,  5, brown);
//   cWS281xDriver::get()->setPixel( 2,  5, brown);
//   cWS281xDriver::get()->setPixel( 1,  5, brown);
//
//   for(int k = 4; k < 12; k++)
//   {
//      cWS281xDriver::get()->setPixel( k,  4, lightBrown);
//      cWS281xDriver::get()->setPixel( k,  3, lightBrown);
//   }
//
//   for(int k = 6; k < 11; k++)
//      cWS281xDriver::get()->setPixel( k,  3, lightBrown);
//
//   cWS281xDriver::get()->setPixel(11 , 10, lightBrown);
//      cWS281xDriver::get()->setPixel( 4 , 10, lightBrown);
//      cWS281xDriver::get()->setPixel(11 ,  9, lightBrown);
//      cWS281xDriver::get()->setPixel( 4 ,  9, lightBrown);
//      cWS281xDriver::get()->setPixel( 6 ,  8, lightBrown);
//      cWS281xDriver::get()->setPixel(11 ,  8, lightBrown);
//      cWS281xDriver::get()->setPixel( 9 ,  8, lightBrown);
//      cWS281xDriver::get()->setPixel( 4 ,  8, lightBrown);
//      cWS281xDriver::get()->setPixel(11 ,  7, lightBrown);
//      cWS281xDriver::get()->setPixel( 4 ,  7, lightBrown);
//      cWS281xDriver::get()->setPixel( 5 ,  7, lightBrown);
//      cWS281xDriver::get()->setPixel(10 ,  7, lightBrown);
//      cWS281xDriver::get()->setPixel( 9 ,  7, lightBrown);
//      cWS281xDriver::get()->setPixel( 6 ,  7, lightBrown);
//
//      cWS281xDriver::get()->setPixel(12 , 11, off);
//      cWS281xDriver::get()->setPixel(11 , 11, off);
//      cWS281xDriver::get()->setPixel(10 , 10, off);
//      cWS281xDriver::get()->setPixel(10 ,  9, off);
//      cWS281xDriver::get()->setPixel(10 ,  8, off);
//      cWS281xDriver::get()->setPixel( 9 ,  9, off);
//      cWS281xDriver::get()->setPixel( 8 ,  9, off);
//      cWS281xDriver::get()->setPixel( 7 ,  9, off);
//      cWS281xDriver::get()->setPixel( 6 ,  9, off);
//      cWS281xDriver::get()->setPixel(4 , 11, off);
//      cWS281xDriver::get()->setPixel(3 , 11, off);
//      cWS281xDriver::get()->setPixel(5 , 10, off);
//      cWS281xDriver::get()->setPixel(5 ,  9, off);
//      cWS281xDriver::get()->setPixel(5 ,  8, off);
//
//      static cyg_bool flag = false;
//   if(flag)
//   {
//   for(int k = 6; k < 11; k++)
//      cWS281xDriver::get()->setPixel( k,  2, lightBrown);
//
//
//   cWS281xDriver::get()->setPixel( 7,  1, lightBrown);
//   cWS281xDriver::get()->setPixel( 8,  1, lightBrown);
//   cWS281xDriver::get()->setPixel( 9,  1, lightBrown);
//
//
//   cWS281xDriver::get()->setPixel( 8,  0, lightBrown);
//
//   cWS281xDriver::get()->setPixel( 2 , 3, off);
//   cWS281xDriver::get()->setPixel( 3 , 3, off);
//   for(int k = 1; k < 6; k++)
//      cWS281xDriver::get()->setPixel( k,  2, off);
//
//   cWS281xDriver::get()->setPixel(11,  2, off);
//   cWS281xDriver::get()->setPixel(12,  2, off);
//
//   for(int k = 1; k < 7; k++)
//      cWS281xDriver::get()->setPixel( k,  1, off);
//
//   cWS281xDriver::get()->setPixel(10,  1, off);
//   cWS281xDriver::get()->setPixel(11,  1, off);
//   cWS281xDriver::get()->setPixel(12,  1, off);
//
//   for(int k = 2; k < 7; k++)
//      cWS281xDriver::get()->setPixel( k,  0, off);
//
//   cWS281xDriver::get()->setPixel( 9,  0, off);
//   cWS281xDriver::get()->setPixel(10,  0, off);
//   cWS281xDriver::get()->setPixel(11,  0, off);
//
//   flag = false;
//   }
//   else
//   {
//      flag = true;
//      for(int k = 5; k < 10; k++)
//            cWS281xDriver::get()->setPixel( k,  2, lightBrown);
//
//         cWS281xDriver::get()->setPixel( 6,  1, lightBrown);
//         cWS281xDriver::get()->setPixel( 7,  1, lightBrown);
//         cWS281xDriver::get()->setPixel( 8,  1, lightBrown);
//
//
//         cWS281xDriver::get()->setPixel( 7,  0, lightBrown);
//
//         cWS281xDriver::get()->setPixel( 12 , 3, off);
//         cWS281xDriver::get()->setPixel( 13 , 3, off);
//         for(int k = 10; k < 15; k++)
//            cWS281xDriver::get()->setPixel( k,  2, off);
//
//         cWS281xDriver::get()->setPixel(3,  2, off);
//         cWS281xDriver::get()->setPixel(4,  2, off);
//
//         for(int k = 9; k < 15; k++)
//            cWS281xDriver::get()->setPixel( k,  1, off);
//
//         cWS281xDriver::get()->setPixel( 3,  1, off);
//         cWS281xDriver::get()->setPixel( 4,  1, off);
//         cWS281xDriver::get()->setPixel( 5,  1, off);
//
//         for(int k = 9; k < 14; k++)
//            cWS281xDriver::get()->setPixel( k,  0, off);
//
//         cWS281xDriver::get()->setPixel( 4,  0, off);
//         cWS281xDriver::get()->setPixel( 5,  0, off);
//         cWS281xDriver::get()->setPixel( 6,  0, off);
//   }

//   cWS281xDriver::get()->paint();

//   cyg_thread_delay(50);
}

GoombaAnimation::~GoombaAnimation() {
   // TODO Auto-generated destructor stub
}

