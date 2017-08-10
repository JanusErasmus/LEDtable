#ifndef BLUE_DEVICE_H_
#define BLUE_DEVICE_H_
#include <hdlc_framer.h>

#include <bluenrg.h>
#include <bluenrg_char.h>

class cButtonListener : public cBlueNRGlistener
{
public:
   cButtonListener(){};
   ~cButtonListener(){};

   void handleData(uint8_t *data, uint8_t len);
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

};

#endif /* BLUE_DEVICE_H_ */
