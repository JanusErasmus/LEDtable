#include "SpiralsAnimation.h"
#include "ws281x_driver.h"

SpiralsAnimation::SpiralsAnimation() :
      string1(15, 8 , 15, 15, 8 , 8),
      string2(7,  8, 7 , 15 , 0 , 8),
      string3(15, 0 , 15, 7 , 8, 0),
      string4(7 , 0 , 7 , 7 , 0, 0)
{
       string2.startColor(6);
       string2.next();
       string2.next();
       string2.next();
       string3.startColor(2);
       string3.next();
       string3.next();
       string4.startColor(4);
       string4.next();
       string4.next();
       string4.next();
       string4.next();
       string4.next();
}

void SpiralsAnimation::run()
{
   string1.run();
   string2.run();
   string3.run();
   string4.run();
   cWS281xDriver::get()->paint();
   cyg_thread_delay(1);

   string1.next();
   string2.next();
   string3.next();
   string4.next();
   string1.run();
   string2.run();
   string3.run();
   string4.run();
   cWS281xDriver::get()->paint();
   cyg_thread_delay(1);

   string1.next();
   string3.next();
   string1.run();
   string3.run();
   cWS281xDriver::get()->paint();
   cyg_thread_delay(1);

   string1.next();
   string1.run();
   cWS281xDriver::get()->paint();
   cyg_thread_delay(1);

}

SpiralsAnimation::~SpiralsAnimation()
{
}

