#ifndef BLUE_DEVICE_H_
#define BLUE_DEVICE_H_
#include <hdlc_framer.h>

#include <bluenrg.h>
#include <bluenrg_char.h>

class cButtonHandler
{
public:
   enum eButton
   {
      UP,
      DOWN,
      LEFT,
      RIGHT
   };

   cButtonHandler(){};
   virtual ~cButtonHandler(){};

   virtual void handleButton(eButton buttun) = 0;
};

class cButtonListener : public cBlueNRGlistener
{
   cButtonHandler *mHandler;

public:
   cButtonListener(){mHandler = 0;};
   ~cButtonListener(){};

   void handleData(uint8_t *data, uint8_t len);

   void setHandler(cButtonHandler *handler){ mHandler = handler; }
};

class cBlueDevice
{
   static cBlueDevice *__instance;
   cBlueNRG *mBlueNRG;

   cButtonListener *mButtonListener;


   cBlueDevice(cBlueNRG *blueNRG);
   virtual ~cBlueDevice();

   void initNVM();
   void initLink();
   void initTransaction();
   void initLock();
   void initDisplay();
   void initGPS();

public:
   static void init(cBlueNRG *blueNRG);
   static cBlueDevice * get();

   void setHandler(cButtonHandler *handler){ mButtonListener->setHandler(handler); }

};

#endif /* BLUE_DEVICE_H_ */
