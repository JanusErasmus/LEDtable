#include "GoombaAnimation.h"
#include "ws281x_driver.h"

GoombaAnimation::GoombaAnimation()
{
}

void GoombaAnimation::run()
{

   cWS281xDriver::get()->setAll(lightBlue);
   cWS281xDriver::get()->setPixel(8 , 15, brown);
   cWS281xDriver::get()->setPixel(7 , 15, brown);
   cWS281xDriver::get()->setPixel(6 , 15, brown);
   cWS281xDriver::get()->setPixel(10, 14, brown);
   cWS281xDriver::get()->setPixel( 9, 14, brown);
   cWS281xDriver::get()->setPixel( 8, 14, brown);
   cWS281xDriver::get()->setPixel( 7, 14, brown);
   cWS281xDriver::get()->setPixel( 6, 14, brown);
   cWS281xDriver::get()->setPixel( 5, 14, brown);
   cWS281xDriver::get()->setPixel(11, 13, brown);
   cWS281xDriver::get()->setPixel(10, 13, brown);
   cWS281xDriver::get()->setPixel( 9, 13, brown);
   cWS281xDriver::get()->setPixel( 8, 13, brown);
   cWS281xDriver::get()->setPixel( 7, 13, brown);
   cWS281xDriver::get()->setPixel( 6, 13, brown);
   cWS281xDriver::get()->setPixel( 5, 13, brown);
   cWS281xDriver::get()->setPixel( 4, 13, brown);
   cWS281xDriver::get()->setPixel(12, 12, brown);
   cWS281xDriver::get()->setPixel(11, 12, brown);
   cWS281xDriver::get()->setPixel(10, 12, brown);
   cWS281xDriver::get()->setPixel( 9, 12, brown);
   cWS281xDriver::get()->setPixel( 8, 12, brown);
   cWS281xDriver::get()->setPixel( 7, 12, brown);
   cWS281xDriver::get()->setPixel( 6, 12, brown);
   cWS281xDriver::get()->setPixel( 5, 12, brown);
   cWS281xDriver::get()->setPixel( 4, 12, brown);
   cWS281xDriver::get()->setPixel( 3, 12, brown);
   cWS281xDriver::get()->setPixel(13, 11, brown);
   cWS281xDriver::get()->setPixel(10, 11, brown);
   cWS281xDriver::get()->setPixel( 9, 11, brown);
   cWS281xDriver::get()->setPixel( 8, 11, brown);
   cWS281xDriver::get()->setPixel( 7, 11, brown);
   cWS281xDriver::get()->setPixel( 6, 11, brown);
   cWS281xDriver::get()->setPixel( 5, 11, brown);
   cWS281xDriver::get()->setPixel( 2, 11, brown);
   cWS281xDriver::get()->setPixel(14, 10, brown);
   cWS281xDriver::get()->setPixel(13, 10, brown);
   cWS281xDriver::get()->setPixel(12, 10, brown);
   cWS281xDriver::get()->setPixel( 9, 10, brown);
   cWS281xDriver::get()->setPixel( 8, 10, brown);
   cWS281xDriver::get()->setPixel( 7, 10, brown);
   cWS281xDriver::get()->setPixel( 6, 10, brown);
   cWS281xDriver::get()->setPixel( 3, 10, brown);
   cWS281xDriver::get()->setPixel( 2, 10, brown);
   cWS281xDriver::get()->setPixel( 1, 10, brown);
   cWS281xDriver::get()->setPixel(14,  9, brown);
   cWS281xDriver::get()->setPixel(13,  9, brown);
   cWS281xDriver::get()->setPixel(12,  9, brown);
   cWS281xDriver::get()->setPixel( 9,  9, brown);
   cWS281xDriver::get()->setPixel( 8,  9, brown);
   cWS281xDriver::get()->setPixel( 7,  9, brown);
   cWS281xDriver::get()->setPixel( 6,  9, brown);
   cWS281xDriver::get()->setPixel( 3,  9, brown);
   cWS281xDriver::get()->setPixel( 2,  9, brown);
   cWS281xDriver::get()->setPixel( 1,  9, brown);
   cWS281xDriver::get()->setPixel(15,  9, brown);
   cWS281xDriver::get()->setPixel(14,  9, brown);
   cWS281xDriver::get()->setPixel(13,  9, brown);
   cWS281xDriver::get()->setPixel(12,  9, brown);
   cWS281xDriver::get()->setPixel( 9,  9, brown);
   cWS281xDriver::get()->setPixel( 8,  9, brown);
   cWS281xDriver::get()->setPixel( 7,  9, brown);
   cWS281xDriver::get()->setPixel( 6,  9, brown);
   cWS281xDriver::get()->setPixel( 3,  9, brown);
   cWS281xDriver::get()->setPixel( 2,  9, brown);
   cWS281xDriver::get()->setPixel( 1,  9, brown);
   cWS281xDriver::get()->setPixel( 0,  9, brown);
   cWS281xDriver::get()->setPixel(15,  8, brown);
   cWS281xDriver::get()->setPixel(14,  8, brown);
   cWS281xDriver::get()->setPixel(13,  8, brown);
   cWS281xDriver::get()->setPixel(12,  8, brown);
   cWS281xDriver::get()->setPixel( 9,  8, brown);
   cWS281xDriver::get()->setPixel( 8,  8, brown);
   cWS281xDriver::get()->setPixel( 7,  8, brown);
   cWS281xDriver::get()->setPixel( 6,  8, brown);
   cWS281xDriver::get()->setPixel( 3,  8, brown);
   cWS281xDriver::get()->setPixel( 2,  8, brown);
   cWS281xDriver::get()->setPixel( 1,  8, brown);
   cWS281xDriver::get()->setPixel( 0,  8, brown);
   cWS281xDriver::get()->setPixel(15,  7, brown);
   cWS281xDriver::get()->setPixel(14,  7, brown);
   cWS281xDriver::get()->setPixel(13,  7, brown);
   cWS281xDriver::get()->setPixel(12,  7, brown);
   cWS281xDriver::get()->setPixel( 9,  7, brown);
   cWS281xDriver::get()->setPixel( 8,  7, brown);
   cWS281xDriver::get()->setPixel( 7,  7, brown);
   cWS281xDriver::get()->setPixel( 6,  7, brown);
   cWS281xDriver::get()->setPixel( 3,  7, brown);
   cWS281xDriver::get()->setPixel( 2,  7, brown);
   cWS281xDriver::get()->setPixel( 1,  7, brown);
   cWS281xDriver::get()->setPixel( 0,  7, brown);

   cWS281xDriver::get()->setPixel(11 , 10, lightBrown);
   cWS281xDriver::get()->setPixel( 4 , 10, lightBrown);
   cWS281xDriver::get()->setPixel(11 ,  9, lightBrown);
   cWS281xDriver::get()->setPixel( 4 ,  9, lightBrown);
   cWS281xDriver::get()->setPixel( 6 ,  8, lightBrown);
   cWS281xDriver::get()->setPixel(11 ,  8, lightBrown);
   cWS281xDriver::get()->setPixel( 9 ,  8, lightBrown);
   cWS281xDriver::get()->setPixel( 4 ,  8, lightBrown);
   cWS281xDriver::get()->setPixel(11 ,  7, lightBrown);
   cWS281xDriver::get()->setPixel( 4 ,  7, lightBrown);
   cWS281xDriver::get()->setPixel( 5 ,  7, lightBrown);
   cWS281xDriver::get()->setPixel(10 ,  7, lightBrown);
   cWS281xDriver::get()->setPixel( 9 ,  7, lightBrown);
   cWS281xDriver::get()->setPixel( 6 ,  7, lightBrown);

   cWS281xDriver::get()->setPixel(12 , 11, off);
   cWS281xDriver::get()->setPixel(11 , 11, off);
   cWS281xDriver::get()->setPixel(10 , 10, off);
   cWS281xDriver::get()->setPixel(10 ,  9, off);
   cWS281xDriver::get()->setPixel(10 ,  8, off);
   cWS281xDriver::get()->setPixel( 9 ,  9, off);
   cWS281xDriver::get()->setPixel( 8 ,  9, off);
   cWS281xDriver::get()->setPixel( 7 ,  9, off);
   cWS281xDriver::get()->setPixel( 6 ,  9, off);
   cWS281xDriver::get()->setPixel(4 , 11, off);
   cWS281xDriver::get()->setPixel(3 , 11, off);
   cWS281xDriver::get()->setPixel(5 , 10, off);
   cWS281xDriver::get()->setPixel(5 ,  9, off);
   cWS281xDriver::get()->setPixel(5 ,  8, off);

   cWS281xDriver::get()->paint();

   cyg_thread_delay(500);
}

GoombaAnimation::~GoombaAnimation() {
   // TODO Auto-generated destructor stub
}

