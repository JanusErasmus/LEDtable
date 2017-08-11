#include <string.h>
#include <stdlib.h>

#include <utils.h>
#include <crc.h>
#include "blue_device.h"

#define TRACE(_x, ...) INFO_TRACE("cBlueDevice", _x,  ##__VA_ARGS__)

cBlueDevice *cBlueDevice::__instance = 0;

void cButtonListener::handleData(uint8_t *data, uint8_t len)
{
   data[len] = 0;

   TRACE("cButtonListener: %d\n", len);
   diag_dump_buf(data, len + 1);


}

void cBlueDevice::init(cBlueNRG *blueNRG)
{
   if(!__instance)
   {
      __instance = new cBlueDevice(blueNRG);
   }
}

cBlueDevice * cBlueDevice::get()
{
   return (cBlueDevice*)__instance;
}

const uint8_t lock_uuid[]   = {0x52, 0xc9, 0x53, 0x67, 0x26, 0x16, 0x04, 0xbd, 0x84, 0x48, 0x14, 0x1c, 0xd0, 0x10, 0x6b, 0x30};
const uint8_t button_uuid[] = {0x52, 0xc9, 0x53, 0x67, 0x26, 0x16, 0x04, 0xbd, 0x84, 0x48, 0x14, 0x1c, 0xd0, 0x10, 0x6b, 0x31};

cBlueDevice::cBlueDevice(cBlueNRG *blueNRG)
{
   mBlueNRG = blueNRG;

   mButtonListener = new cButtonListener();
   cBlueNRGservice * service = new cBlueNRGservice(lock_uuid);
   if(mBlueNRG->addService(service, 1))
   {
      cBlueNRGchar *buttonChar = new cBlueNRGchar(button_uuid, CHAR_PROP_NOTIFY | CHAR_PROP_WRITE_WITHOUT_RESP, 7);
      buttonChar->setListener(mButtonListener);


      if(!service->add(buttonChar))
         TRACE(RED("Lock NOT added\n"));
   }

   TRACE("All services\n");
   mBlueNRG->listServices();

   mBlueNRG->setConnectable();
}


cBlueDevice::~cBlueDevice()
{
}

